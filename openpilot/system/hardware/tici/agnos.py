#!/usr/bin/env python3
import hashlib
import json
import lzma
import os
from pathlib import Path
import shutil
import struct
import subprocess
import threading
import time
from collections.abc import Generator
from concurrent.futures import ThreadPoolExecutor, as_completed

import requests

SPARSE_CHUNK_FMT = struct.Struct('H2xI4x')
CAIBX_URL = "https://commadist.azureedge.net/agnosupdate/"

AGNOS_MANIFEST_FILE = "openpilot/system/hardware/tici/agnos.json"
DOWNLOAD_CACHE_DIR = Path(os.getenv("AGNOS_DOWNLOAD_CACHE_DIR", "/data/agnos-update-cache"))

# --- AGNOS image download acceleration ----------------------------------
# The boot / system partitions live on GitHub Releases. From mainland China a
# single connection to those hosts often runs at 20-100 KB/s, and the system
# partition is ~1 GB compressed, so a fresh install stalls here for hours.
#
# The manifest URL stays the one authoritative source. On top of it we:
#   1) probe a few GitHub mirror prefixes and keep whichever answers fastest,
#   2) fetch the image with several parallel ranged requests, resumable per
#      chunk, which multiplies throughput on throttled routes.
# Integrity is still enforced by the compressed_hash / hash / hash_raw sha256
# checks below, so a misbehaving mirror can only make the download fail, never
# corrupt the image that gets flashed.
#
# Disable the mirrors: AGNOS_MIRROR_PREFIXES="" (empty = manifest URL only).
# Tune concurrency:   AGNOS_DOWNLOAD_STREAMS=4
MIRROR_CONFIG_FILE = Path(os.getenv("AGNOS_MIRROR_CONFIG", "/data/agnos-mirrors.txt"))
DEFAULT_MIRROR_PREFIXES = (
  "https://gh-proxy.com/",
  "https://ghfast.top/",
)
SPEED_PROBE_BYTES = 512 * 1024
SPEED_PROBE_TIMEOUT = 20
PARALLEL_MIN_BYTES = 4 * 1024 * 1024
CHUNK_MIN_BYTES = 2 * 1024 * 1024


def _env_int(name: str, default: int) -> int:
  try:
    return max(1, int(os.getenv(name, str(default))))
  except ValueError:
    return default


DOWNLOAD_STREAMS = _env_int("AGNOS_DOWNLOAD_STREAMS", 8)


class StreamingDecompressor:
  def __init__(self, url: str, cache_path: Path | None = None) -> None:
    self.buf = b""

    self.source = cache_path.open("rb") if cache_path is not None else None
    self.req = None if self.source is not None else requests.get(
      url, stream=True, headers={'Accept-Encoding': None}, timeout=60,
    )
    self.it = (iter(lambda: self.source.read(1024 * 1024), b"") if self.source is not None
               else self.req.iter_content(chunk_size=1024 * 1024))
    self.decompressor = lzma.LZMADecompressor(format=lzma.FORMAT_AUTO)
    self.eof = False
    self.sha256 = hashlib.sha256()

  def read(self, length: int) -> bytes:
    while len(self.buf) < length and not self.eof:
      if self.decompressor.needs_input:
        if self.req is not None:
          self.req.raise_for_status()

        try:
          compressed = next(self.it)
        except StopIteration:
          self.eof = True
          break
      else:
        compressed = b''

      self.buf += self.decompressor.decompress(compressed, max_length=length)

      if self.decompressor.eof:
        self.eof = True
        break

    result = self.buf[:length]
    self.buf = self.buf[length:]

    self.sha256.update(result)
    return result

  def close(self) -> None:
    if self.source is not None:
      self.source.close()
    if self.req is not None:
      self.req.close()


def file_checksum(path: Path) -> str:
  digest = hashlib.sha256()
  with path.open("rb") as source:
    for chunk in iter(lambda: source.read(1024 * 1024), b""):
      digest.update(chunk)
  return digest.hexdigest()


def _mirror_prefixes() -> tuple[str, ...]:
  """Mirror prefixes: env var > config file > built-in defaults. Empty means off."""
  env = os.getenv("AGNOS_MIRROR_PREFIXES")
  if env is not None:
    return tuple(prefix.strip() for prefix in env.split(",") if prefix.strip())
  try:
    if MIRROR_CONFIG_FILE.is_file():
      lines = MIRROR_CONFIG_FILE.read_text(encoding="utf-8").splitlines()
      return tuple(line.strip() for line in lines if line.strip() and not line.strip().startswith("#"))
  except OSError:
    pass
  return DEFAULT_MIRROR_PREFIXES


def _candidate_urls(url: str) -> list[str]:
  """The manifest URL first, then the same URL behind each mirror prefix."""
  candidates = [url]
  for prefix in _mirror_prefixes():
    candidates.append(prefix + url)

  seen: set[str] = set()
  unique = []
  for candidate in candidates:
    if candidate not in seen:
      seen.add(candidate)
      unique.append(candidate)
  return unique


def _speed_probe(url: str) -> tuple[float | None, bool]:
  """Grab a small ranged slice. Returns (MB/s or None, server honoured the range)."""
  try:
    started = time.time()
    response = requests.get(url, stream=True, timeout=SPEED_PROBE_TIMEOUT,
                            headers={'Accept-Encoding': None,
                                     'Range': f'bytes=0-{SPEED_PROBE_BYTES - 1}'})
    response.raise_for_status()
    ranged = response.status_code == 206
    received = 0
    try:
      for chunk in response.iter_content(chunk_size=64 * 1024):
        received += len(chunk)
        if received >= SPEED_PROBE_BYTES or time.time() - started > SPEED_PROBE_TIMEOUT:
          break
    finally:
      response.close()
    if received < SPEED_PROBE_BYTES // 2:
      return None, ranged
    return received / 1024 / 1024 / max(time.time() - started, 0.05), ranged
  except Exception:
    return None, False


def _choose_source(urls: list[str], cloudlog, want_parallel: bool) -> tuple[str, bool]:
  """Pick the fastest reachable source. Returns (url, use_parallel)."""
  if len(urls) < 2:
    return urls[0], want_parallel

  speeds: dict[str, float] = {}
  ranged: dict[str, bool] = {}
  with ThreadPoolExecutor(max_workers=len(urls)) as pool:
    futures = {pool.submit(_speed_probe, url): url for url in urls}
    for future in as_completed(futures):
      url = futures[future]
      speed, honoured_range = future.result()
      if speed is not None:
        speeds[url] = speed
        ranged[url] = honoured_range

  if not speeds:
    cloudlog.warning("No AGNOS source answered the speed probe; using the manifest URL")
    return urls[0], want_parallel

  cloudlog.info("AGNOS source probe (MB/s): " + ", ".join(
    f"{url.split('/')[2]}={speeds[url]:.3f}" for url in sorted(speeds, key=speeds.get, reverse=True)))

  if want_parallel:
    parallel_ready = {url: speed for url, speed in speeds.items() if ranged.get(url)}
    if parallel_ready:
      best = max(parallel_ready, key=parallel_ready.get)
      return best, True
    # Fastest source cannot do ranged requests, so it has to go over one stream.
    return max(speeds, key=speeds.get), False

  return max(speeds, key=speeds.get), False


def _chunk_bounds(total: int, streams: int) -> list[tuple[int, int]]:
  """Split [0, total) into closed intervals, one per stream."""
  streams = max(1, min(streams, max(1, total // CHUNK_MIN_BYTES)))
  base = total // streams
  bounds = []
  position = 0
  for index in range(streams):
    size = base if index < streams - 1 else total - position
    bounds.append((position, position + size - 1))
    position += size
  return bounds


def _chunk_paths(partial_path: Path, count: int) -> list[Path]:
  suffix = partial_path.suffix
  return [partial_path.with_suffix(suffix + f'.chunk{index:02d}') for index in range(count)]


def _cleanup_chunks(partial_path: Path) -> None:
  for path in partial_path.parent.glob(partial_path.name + '.chunk*'):
    path.unlink(missing_ok=True)


def _download_parallel(url: str, partial_path: Path, total: int, name: str, cloudlog) -> None:
  """Fetch `total` bytes with parallel ranged requests, then concatenate.

  Each chunk lives in its own file, so its size doubles as resume state: an
  interrupted download only re-fetches the chunks that are still short.
  A pre-existing sequential .part file (from the single-stream path) is
  adopted as the first chunk to avoid re-downloading it.
  """
  bounds = _chunk_bounds(total, DOWNLOAD_STREAMS)
  chunks = _chunk_paths(partial_path, len(bounds))

  if partial_path.is_file() and not chunks[0].exists():
    pending = partial_path.stat().st_size
    first_chunk_size = bounds[0][1] - bounds[0][0] + 1
    if 0 < pending <= first_chunk_size:
      os.replace(partial_path, chunks[0])
    else:
      partial_path.unlink(missing_ok=True)

  completed = 0
  last_reported = -1
  progress_lock = threading.Lock()

  def report(increment: int) -> None:
    nonlocal completed, last_reported
    with progress_lock:
      completed += increment
      percent = int(completed / total * 100)
      if percent != last_reported:
        last_reported = percent
        print(f"Downloading {name}: {percent}", flush=True)

  def worker(start: int, end: int, path: Path) -> None:
    want = end - start + 1
    have = path.stat().st_size if path.is_file() else 0
    if have > want:
      path.unlink(missing_ok=True)
      have = 0
    if have == want:
      report(want)
      return

    # Every chunk asks for its own byte range so a stream never pulls more than
    # the slice it is responsible for. Without this, a chunk other than the
    # first would receive the head of the file and corrupt the concatenation.
    request_start = start + have
    response = requests.get(url, stream=True, timeout=60,
                            headers={'Accept-Encoding': None,
                                     'Range': f'bytes={request_start}-{end}'})
    try:
      response.raise_for_status()
      if response.status_code != 206 and request_start > 0:
        # Server ignored the range; reading it would write the wrong bytes.
        raise requests.ConnectionError(
          f"{name}: server ignored the range request for chunk {path.name}")

      with path.open('ab' if have else 'wb') as output:
        report(have)
        received = have
        for chunk in response.iter_content(chunk_size=1024 * 1024):
          if not chunk:
            continue
          output.write(chunk)
          received += len(chunk)
          report(len(chunk))
          if received >= want:
            break
        output.flush()
        os.fsync(output.fileno())
    finally:
      response.close()

    if path.stat().st_size != want:
      raise requests.ConnectionError(
        f"{name} chunk {path.name} short: {path.stat().st_size} of {want} bytes")

  with ThreadPoolExecutor(max_workers=len(bounds)) as pool:
    futures = [pool.submit(worker, start, end, path)
               for (start, end), path in zip(bounds, chunks)]
    for future in futures:
      future.result()

  with partial_path.open('wb') as output:
    for path in chunks:
      with path.open('rb') as source:
        shutil.copyfileobj(source, output, 1024 * 1024)
    output.flush()
    os.fsync(output.fileno())

  for path in chunks:
    path.unlink(missing_ok=True)


def _download_single(url: str, partial_path: Path, offset: int, expected_size: int | None,
                     name: str, cloudlog) -> None:
  """Sequential download, resumable from `offset` via a Range request."""
  headers: dict[str, str | None] = {'Accept-Encoding': None}
  if offset:
    headers['Range'] = f"bytes={offset}-"

  response = requests.get(url, stream=True, headers=headers, timeout=60)
  response.raise_for_status()

  if offset and response.status_code != 206:
    cloudlog.warning(f"Server ignored resume for {name}; restarting the cached download")
    offset = 0

  if expected_size is None:
    content_length = response.headers.get("Content-Length")
    expected_size = offset + int(content_length) if content_length is not None else None

  mode = "ab" if offset else "wb"
  last_p = -1
  try:
    with partial_path.open(mode) as output:
      for chunk in response.iter_content(chunk_size=1024 * 1024):
        if not chunk:
          continue
        output.write(chunk)
        if expected_size:
          p = int(output.tell() / expected_size * 100)
          if p != last_p:
            last_p = p
            print(f"Downloading {name}: {p}", flush=True)
      output.flush()
      os.fsync(output.fileno())
  finally:
    response.close()


def download_to_cache(partition: dict, cloudlog) -> Path | None:
  compressed_hash = partition.get("compressed_hash")
  if not isinstance(compressed_hash, str):
    return None

  DOWNLOAD_CACHE_DIR.mkdir(parents=True, exist_ok=True)
  final_path = DOWNLOAD_CACHE_DIR / f"{partition['name']}-{compressed_hash}.img.xz"
  partial_path = final_path.with_suffix(final_path.suffix + ".part")
  if final_path.is_file():
    if file_checksum(final_path).lower() == compressed_hash.lower():
      return final_path
    cloudlog.warning(f"Discarding invalid cached {partition['name']} image")
    final_path.unlink()

  offset = partial_path.stat().st_size if partial_path.is_file() else 0
  expected_size = partition.get("compressed_size")
  if not isinstance(expected_size, int):
    expected_size = None
  if expected_size is not None and offset >= expected_size:
    if offset == expected_size and file_checksum(partial_path).lower() == compressed_hash.lower():
      os.replace(partial_path, final_path)
      return final_path
    cloudlog.warning(f"Discarding invalid partial {partition['name']} image")
    partial_path.unlink()
    offset = 0

  name = partition['name']
  want_parallel = expected_size is not None and expected_size >= PARALLEL_MIN_BYTES
  if want_parallel:
    url, use_parallel = _choose_source(_candidate_urls(partition['url']), cloudlog, True)
  else:
    url, use_parallel = partition['url'], False

  attempts = [(url, use_parallel)]
  if url != partition['url']:
    # Fall back to the manifest URL if the probe picked a mirror that then failed.
    attempts.append((partition['url'], False))

  error: Exception | None = None
  for attempt_url, attempt_parallel in attempts:
    host = attempt_url.split('/')[2]
    try:
      if attempt_parallel:
        cloudlog.info(f"Downloading {name} ({expected_size} B) in parallel from {host}")
        _download_parallel(attempt_url, partial_path, expected_size, name, cloudlog)
      else:
        cloudlog.info(f"Downloading {name} cache from byte {offset} via {host}")
        _download_single(attempt_url, partial_path, offset, expected_size, name, cloudlog)
      error = None
      break
    except requests.exceptions.RequestException as exc:
      error = exc
      cloudlog.warning(f"AGNOS source {host} failed for {name}: {exc}")

  if error is not None:
    raise error

  downloaded_size = partial_path.stat().st_size
  if expected_size is not None and downloaded_size != expected_size:
    raise requests.ConnectionError(
      f"Incomplete {name} download: {downloaded_size} of {expected_size} bytes"
    )
  actual_hash = file_checksum(partial_path)
  if actual_hash.lower() != compressed_hash.lower():
    partial_path.unlink(missing_ok=True)
    _cleanup_chunks(partial_path)
    raise requests.ConnectionError(f"Compressed {name} cache hash mismatch: {actual_hash}")

  os.replace(partial_path, final_path)
  return final_path


def unsparsify(f: StreamingDecompressor) -> Generator[bytes, None, None]:
  # https://source.android.com/devices/bootloader/images#sparse-format
  magic = struct.unpack("I", f.read(4))[0]
  assert(magic == 0xed26ff3a)

  # Version
  major = struct.unpack("H", f.read(2))[0]
  minor = struct.unpack("H", f.read(2))[0]
  assert(major == 1 and minor == 0)

  f.read(2)  # file header size
  f.read(2)  # chunk header size

  block_sz = struct.unpack("I", f.read(4))[0]
  f.read(4)  # total blocks
  num_chunks = struct.unpack("I", f.read(4))[0]
  f.read(4)  # crc checksum

  for _ in range(num_chunks):
    chunk_type, out_blocks = SPARSE_CHUNK_FMT.unpack(f.read(12))

    if chunk_type == 0xcac1:  # Raw
      # TODO: yield in smaller chunks. Yielding only block_sz is too slow. Largest observed data chunk is 252 MB.
      yield f.read(out_blocks * block_sz)
    elif chunk_type == 0xcac2:  # Fill
      filler = f.read(4) * (block_sz // 4)
      for _ in range(out_blocks):
        yield filler
    elif chunk_type == 0xcac3:  # Don't care
      yield b""
    else:
      raise Exception("Unhandled sparse chunk type")


# noop wrapper with same API as unsparsify() for non sparse images
def noop(f: StreamingDecompressor) -> Generator[bytes, None, None]:
  while len(chunk := f.read(1024 * 1024)) > 0:
    yield chunk


def get_target_slot_number() -> int:
  current_slot = subprocess.check_output(["abctl", "--boot_slot"], encoding='utf-8').strip()
  return 1 if current_slot == "_a" else 0


def slot_number_to_suffix(slot_number: int) -> str:
  assert slot_number in (0, 1)
  return '_a' if slot_number == 0 else '_b'


def get_partition_path(target_slot_number: int, partition: dict) -> str:
  path = f"/dev/disk/by-partlabel/{partition['name']}"

  if partition.get('has_ab', True):
    path += slot_number_to_suffix(target_slot_number)

  return path


def get_raw_hash(path: str, partition_size: int) -> str:
  raw_hash = hashlib.sha256()
  pos, chunk_size = 0, 1024 * 1024

  with open(path, 'rb+') as out:
    while pos < partition_size:
      n = min(chunk_size, partition_size - pos)
      raw_hash.update(out.read(n))
      pos += n

  return raw_hash.hexdigest().lower()


def verify_partition(target_slot_number: int, partition: dict[str, str | int], force_full_check: bool = False) -> bool:
  full_check = partition['full_check'] or force_full_check
  path = get_partition_path(target_slot_number, partition)

  if not isinstance(partition['size'], int):
    return False

  partition_size: int = partition['size']

  if not isinstance(partition['hash_raw'], str):
    return False

  partition_hash: str = partition['hash_raw']

  if full_check:
    return get_raw_hash(path, partition_size) == partition_hash.lower()
  else:
    with open(path, 'rb+') as out:
      out.seek(partition_size)
      return out.read(64) == partition_hash.lower().encode()


def clear_partition_hash(target_slot_number: int, partition: dict) -> None:
  path = get_partition_path(target_slot_number, partition)
  with open(path, 'wb+') as out:
    partition_size = partition['size']

    out.seek(partition_size)
    out.write(b"\x00" * 64)
    os.sync()


def extract_compressed_image(target_slot_number: int, partition: dict, cloudlog):
  path = get_partition_path(target_slot_number, partition)
  cache_path = download_to_cache(partition, cloudlog)
  downloader = StreamingDecompressor(partition['url'], cache_path)

  try:
    with open(path, 'wb+') as out:
      # Flash partition
      last_p = 0
      raw_hash = hashlib.sha256()
      f = unsparsify if partition['sparse'] else noop
      for chunk in f(downloader):
        raw_hash.update(chunk)
        out.write(chunk)
        p = int(out.tell() / partition['size'] * 100)
        if p != last_p:
          last_p = p
          print(f"Installing {partition['name']}: {p}", flush=True)

      if raw_hash.hexdigest().lower() != partition['hash_raw'].lower():
        raise Exception(f"Raw hash mismatch '{raw_hash.hexdigest().lower()}'")

      if downloader.sha256.hexdigest().lower() != partition['hash'].lower():
        raise Exception("Uncompressed hash mismatch")

      if out.tell() != partition['size']:
        raise Exception("Uncompressed size mismatch")

      os.sync()
  finally:
    downloader.close()

  if cache_path is not None:
    cache_path.unlink(missing_ok=True)


def extract_casync_image(target_slot_number: int, partition: dict, cloudlog):
  # The standalone updater does not use casync. Import it only for this path
  # so recovery/standalone flashing does not require pycryptodome.
  import openpilot.system.updated.casync.casync as casync

  path = get_partition_path(target_slot_number, partition)
  seed_path = path[:-1] + ('b' if path[-1] == 'a' else 'a')

  target = casync.parse_caibx(partition['casync_caibx'])

  sources: list[tuple[str, casync.ChunkReader, casync.ChunkDict]] = []

  # First source is the current partition.
  try:
    raw_hash = get_raw_hash(seed_path, partition['size'])
    caibx_url = f"{CAIBX_URL}{partition['name']}-{raw_hash}.caibx"

    try:
      cloudlog.info(f"casync fetching {caibx_url}")
      sources += [('seed', casync.FileChunkReader(seed_path), casync.build_chunk_dict(casync.parse_caibx(caibx_url)))]
    except requests.RequestException:
      cloudlog.error(f"casync failed to load {caibx_url}")
  except Exception:
    cloudlog.exception("casync failed to hash seed partition")

  # Second source is the target partition, this allows for resuming
  sources += [('target', casync.FileChunkReader(path), casync.build_chunk_dict(target))]

  # Finally we add the remote source to download any missing chunks
  sources += [('remote', casync.RemoteChunkReader(partition['casync_store']), casync.build_chunk_dict(target))]

  last_p = 0

  def progress(cur):
    nonlocal last_p
    p = int(cur / partition['size'] * 100)
    if p != last_p:
      last_p = p
      print(f"Installing {partition['name']}: {p}", flush=True)

  stats = casync.extract(target, sources, path, progress)
  cloudlog.error(f'casync done {json.dumps(stats)}')

  os.sync()
  if not verify_partition(target_slot_number, partition, force_full_check=True):
    raise Exception(f"Raw hash mismatch '{partition['hash_raw'].lower()}'")


def flash_partition(target_slot_number: int, partition: dict, cloudlog, standalone=False):
  cloudlog.info(f"Downloading and writing {partition['name']}")

  if verify_partition(target_slot_number, partition):
    cloudlog.info(f"Already flashed {partition['name']}")
    return

  # Clear hash before flashing in case we get interrupted
  full_check = partition['full_check']
  if not full_check:
    clear_partition_hash(target_slot_number, partition)

  path = get_partition_path(target_slot_number, partition)

  if ('casync_caibx' in partition) and not standalone:
    extract_casync_image(target_slot_number, partition, cloudlog)
  else:
    extract_compressed_image(target_slot_number, partition, cloudlog)

  # Write hash after successful flash
  if not full_check:
    with open(path, 'wb+') as out:
      out.seek(partition['size'])
      out.write(partition['hash_raw'].lower().encode())


def swap(manifest_path: str, target_slot_number: int, cloudlog) -> None:
  update = json.load(open(manifest_path))
  for partition in update:
    if not partition.get('full_check', False):
      clear_partition_hash(target_slot_number, partition)

  while True:
    out = subprocess.check_output(f"abctl --set_active {target_slot_number}", shell=True, stderr=subprocess.STDOUT, encoding='utf8')
    if ("No such file or directory" not in out) and ("lun as boot lun" in out):
      cloudlog.info(f"Swap successful {out}")
      break
    else:
      cloudlog.error(f"Swap failed {out}")


def flash_agnos_update(manifest_path: str, target_slot_number: int, cloudlog, standalone=False) -> None:
  update = json.load(open(manifest_path))

  cloudlog.info(f"Target slot {target_slot_number}")

  # set target slot as unbootable
  os.system(f"abctl --set_unbootable {target_slot_number}")

  for partition in update:
    success = False

    for retries in range(10):
      try:
        flash_partition(target_slot_number, partition, cloudlog, standalone)
        success = True
        break

      except requests.exceptions.RequestException:
        cloudlog.exception("Failed")
        cloudlog.info(f"Failed to download {partition['name']}, retrying ({retries})")
        time.sleep(10)

    if not success:
      cloudlog.info(f"Failed to flash {partition['name']}, aborting")
      raise Exception("Maximum retries exceeded")

  cloudlog.info(f"AGNOS ready on slot {target_slot_number}")


def verify_agnos_update(manifest_path: str, target_slot_number: int) -> bool:
  update = json.load(open(manifest_path))
  return all(verify_partition(target_slot_number, partition) for partition in update)


if __name__ == "__main__":
  import argparse
  import logging

  parser = argparse.ArgumentParser(description="Flash and verify AGNOS update",
                                   formatter_class=argparse.ArgumentDefaultsHelpFormatter)

  parser.add_argument("--verify", action="store_true", help="Verify and perform swap if update ready")
  parser.add_argument("--swap", action="store_true", help="Verify and perform swap, downloads if necessary")
  parser.add_argument("manifest", help="Manifest json")
  args = parser.parse_args()

  logging.basicConfig(level=logging.INFO)

  target_slot_number = get_target_slot_number()
  if args.verify:
    if verify_agnos_update(args.manifest, target_slot_number):
      swap(args.manifest, target_slot_number, logging)
      exit(0)
    exit(1)
  elif args.swap:
    while not verify_agnos_update(args.manifest, target_slot_number):
      logging.error("Verification failed. Flashing AGNOS")
      flash_agnos_update(args.manifest, target_slot_number, logging, standalone=True)

    logging.warning(f"Verification succeeded. Swapping to slot {target_slot_number}")
    swap(args.manifest, target_slot_number, logging)
  else:
    flash_agnos_update(args.manifest, target_slot_number, logging, standalone=True)
