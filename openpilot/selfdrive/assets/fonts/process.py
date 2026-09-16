#!/usr/bin/env python3
from pathlib import Path
import ast
import json

import pyray as rl

FONT_DIR = Path(__file__).resolve().parent
SELFDRIVE_DIR = FONT_DIR.parents[1]
TRANSLATIONS_DIR = SELFDRIVE_DIR / "ui" / "translations"
LANGUAGES_FILE = TRANSLATIONS_DIR / "languages.json"

# 萝卜设置面板的文案来源（见 _extra_ui_chars）
CARROT_SETTINGS_FILE = SELFDRIVE_DIR / "carrot_settings.json"
SETTINGS_UI_GLOB = "ui/layouts/settings/*.py"

GLYPH_PADDING = 6
EXTRA_CHARS = "–‑✓×°§•X⚙✕◀▶✔⌫⇧␣○●↳çêüñ–‑✓×°§•€£¥"
UNIFONT_LANGUAGES = {"th", "zh-CHT", "zh-CHS", "ko", "ja"}
KR_FONTS = {"KaiGenGothicKR-Bold"}

def _languages():
  if not LANGUAGES_FILE.exists():
    return {}
  with LANGUAGES_FILE.open(encoding="utf-8") as f:
    return json.load(f)


def _char_sets():
  base = set(map(chr, range(32, 127))) | set(EXTRA_CHARS)
  unifont = set(base)

  unifont.update(map(chr, range(0xAC00, 0xD7A4)))
  for language, code in _languages().items():
    unifont.update(language)
    po_path = TRANSLATIONS_DIR / f"app_{code}.po"
    try:
      chars = set(po_path.read_text(encoding="utf-8"))
    except FileNotFoundError:
      continue
    (unifont if code in UNIFONT_LANGUAGES else base).update(chars)

  unifont.update(_extra_ui_chars())

  return tuple(sorted(ord(c) for c in base)), tuple(sorted(ord(c) for c in unifont))


def _collect_strings(value, out: set[str]) -> None:
  if isinstance(value, str):
    out.update(value)
  elif isinstance(value, list):
    for item in value:
      _collect_strings(item, out)
  elif isinstance(value, dict):
    for item in value.values():
      _collect_strings(item, out)


def _source_literal_chars(path: Path) -> set[str]:
  """设置页源码里字符串字面量用到的字符（跳过注释，避免把注释文字也烘进图集）"""
  try:
    tree = ast.parse(path.read_text(encoding="utf-8"))
  except (OSError, SyntaxError, ValueError):
    return set()
  chars: set[str] = set()
  for node in ast.walk(tree):
    if isinstance(node, ast.Constant) and isinstance(node.value, str):
      chars.update(node.value)
  return chars


def _extra_ui_chars() -> set[str]:
  """萝卜设置面板会渲染、但 .po 翻译文件里没有的非 ASCII 字符。

  字体图集是构建期烘死的，原来的字符集只来自 .po；而「萝卜」面板的文案来自
  carrot_settings.json（中文标题/描述/选项）和设置页源码里的字面量，两者都不在
  .po 里，缺字会在设备上直接渲染成空白。这里把它们并进 unifont 字符集。
  """
  chars: set[str] = set()

  try:
    with CARROT_SETTINGS_FILE.open(encoding="utf-8") as f:
      _collect_strings(json.load(f), chars)
  except (OSError, ValueError):
    pass

  for source in sorted(SELFDRIVE_DIR.glob(SETTINGS_UI_GLOB)):
    chars.update(_source_literal_chars(source))

  chars.add("·")  # 服务端用 " · " 拼接分节标题，分隔符必须能渲染
  return {c for c in chars if ord(c) > 127 and c.isprintable()}


def _glyph_metrics(glyphs, rects, glyph_count: int):
  entries = []
  min_offset_y, max_extent = None, 0
  for idx in range(glyph_count):
    glyph = glyphs[idx]
    rect = rects[idx]
    width = int(round(rect.width))
    height = int(round(rect.height))
    offset_y = int(round(glyph.offsetY))
    min_offset_y = offset_y if min_offset_y is None else min(min_offset_y, offset_y)
    max_extent = max(max_extent, offset_y + height)
    entries.append({
      "id": glyph.value,
      "x": int(round(rect.x)),
      "y": int(round(rect.y)),
      "width": width,
      "height": height,
      "xoffset": int(round(glyph.offsetX)),
      "yoffset": offset_y,
      "xadvance": int(round(glyph.advanceX)),
    })

  if min_offset_y is None:
    raise RuntimeError("No glyphs were generated")

  line_height = int(round(max_extent - min_offset_y))
  base = int(round(max_extent))
  return entries, line_height, base


def _write_bmfont(path: Path, font_size: int, face: str, atlas_name: str, line_height: int, base: int, atlas_size, entries):
  # TODO: why doesn't raylib calculate these metrics correctly?
  if line_height != font_size:
    print("using font size for line height", atlas_name)
    line_height = font_size
  lines = [
    f"info face=\"{face}\" size=-{font_size} bold=0 italic=0 charset=\"\" unicode=1 stretchH=100 smooth=0 aa=1 padding=0,0,0,0 spacing=0,0 outline=0",
    f"common lineHeight={line_height} base={base} scaleW={atlas_size[0]} scaleH={atlas_size[1]} pages=1 packed=0 alphaChnl=0 redChnl=4 greenChnl=4 blueChnl=4",
    f"page id=0 file=\"{atlas_name}\"",
    f"chars count={len(entries)}",
  ]
  for entry in entries:
    lines.append(
      ("char id={id:<4} x={x:<5} y={y:<5} width={width:<5} height={height:<5} " +
       "xoffset={xoffset:<5} yoffset={yoffset:<5} xadvance={xadvance:<5} page=0  chnl=15").format(**entry)
    )
  path.write_text("\n".join(lines) + "\n")


def _process_font(font_path: Path, codepoints: tuple[int, ...]):
  print(f"Processing {font_path.name}...")

  #font_size = {
  #  "unifont.otf": 16,  # unifont is only 16x8 or 16x16 pixels per glyph
  #}.get(font_path.name, 200)
  font_size = 200
  padding = GLYPH_PADDING

  if font_path.name == "unifont.otf":
    font_size = 16
    padding = GLYPH_PADDING

  if font_path.stem in KR_FONTS:
    font_size = 48
    padding = 2
    
  data = font_path.read_bytes()
  file_buf = rl.ffi.new("unsigned char[]", data)
  cp_buffer = rl.ffi.new("int[]", codepoints)
  cp_ptr = rl.ffi.cast("int *", cp_buffer)
  glyph_count = rl.ffi.new("int *", len(codepoints))
  glyphs = rl.load_font_data(
    rl.ffi.cast("unsigned char *", file_buf), len(data), font_size, cp_ptr, len(codepoints),
    rl.FontType.FONT_DEFAULT, glyph_count
  )
  if glyphs == rl.ffi.NULL:
    raise RuntimeError("raylib failed to load font data")

  rects_ptr = rl.ffi.new("Rectangle **")
  image = rl.gen_image_font_atlas(glyphs, rects_ptr, glyph_count[0], font_size, padding, 0)
  if image.width == 0 or image.height == 0:
    raise RuntimeError("raylib returned an empty atlas")

  rects = rects_ptr[0]
  atlas_name = f"{font_path.stem}.png"
  atlas_path = FONT_DIR / atlas_name
  entries, line_height, base = _glyph_metrics(glyphs, rects, glyph_count[0])

  if not rl.export_image(image, atlas_path.as_posix()):
    raise RuntimeError("Failed to export atlas image")

  _write_bmfont(FONT_DIR / f"{font_path.stem}.fnt", font_size, font_path.stem, atlas_name, line_height, base, (image.width, image.height), entries)


def main():
  base_cp, unifont_cp = _char_sets()
  fonts = sorted(FONT_DIR.glob("*.ttf")) + sorted(FONT_DIR.glob("*.otf"))
  for font in fonts:
    if "emoji" in font.name.lower():
      continue
    glyphs = unifont_cp if font.stem.lower().startswith("unifont") or font.stem in KR_FONTS else base_cp
    _process_font(font, glyphs)
  return 0


if __name__ == "__main__":
  raise SystemExit(main())
