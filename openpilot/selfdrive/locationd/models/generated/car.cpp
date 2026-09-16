#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_7079130866779850545) {
   out_7079130866779850545[0] = delta_x[0] + nom_x[0];
   out_7079130866779850545[1] = delta_x[1] + nom_x[1];
   out_7079130866779850545[2] = delta_x[2] + nom_x[2];
   out_7079130866779850545[3] = delta_x[3] + nom_x[3];
   out_7079130866779850545[4] = delta_x[4] + nom_x[4];
   out_7079130866779850545[5] = delta_x[5] + nom_x[5];
   out_7079130866779850545[6] = delta_x[6] + nom_x[6];
   out_7079130866779850545[7] = delta_x[7] + nom_x[7];
   out_7079130866779850545[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_6372496743972128365) {
   out_6372496743972128365[0] = -nom_x[0] + true_x[0];
   out_6372496743972128365[1] = -nom_x[1] + true_x[1];
   out_6372496743972128365[2] = -nom_x[2] + true_x[2];
   out_6372496743972128365[3] = -nom_x[3] + true_x[3];
   out_6372496743972128365[4] = -nom_x[4] + true_x[4];
   out_6372496743972128365[5] = -nom_x[5] + true_x[5];
   out_6372496743972128365[6] = -nom_x[6] + true_x[6];
   out_6372496743972128365[7] = -nom_x[7] + true_x[7];
   out_6372496743972128365[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_2257575929542866463) {
   out_2257575929542866463[0] = 1.0;
   out_2257575929542866463[1] = 0.0;
   out_2257575929542866463[2] = 0.0;
   out_2257575929542866463[3] = 0.0;
   out_2257575929542866463[4] = 0.0;
   out_2257575929542866463[5] = 0.0;
   out_2257575929542866463[6] = 0.0;
   out_2257575929542866463[7] = 0.0;
   out_2257575929542866463[8] = 0.0;
   out_2257575929542866463[9] = 0.0;
   out_2257575929542866463[10] = 1.0;
   out_2257575929542866463[11] = 0.0;
   out_2257575929542866463[12] = 0.0;
   out_2257575929542866463[13] = 0.0;
   out_2257575929542866463[14] = 0.0;
   out_2257575929542866463[15] = 0.0;
   out_2257575929542866463[16] = 0.0;
   out_2257575929542866463[17] = 0.0;
   out_2257575929542866463[18] = 0.0;
   out_2257575929542866463[19] = 0.0;
   out_2257575929542866463[20] = 1.0;
   out_2257575929542866463[21] = 0.0;
   out_2257575929542866463[22] = 0.0;
   out_2257575929542866463[23] = 0.0;
   out_2257575929542866463[24] = 0.0;
   out_2257575929542866463[25] = 0.0;
   out_2257575929542866463[26] = 0.0;
   out_2257575929542866463[27] = 0.0;
   out_2257575929542866463[28] = 0.0;
   out_2257575929542866463[29] = 0.0;
   out_2257575929542866463[30] = 1.0;
   out_2257575929542866463[31] = 0.0;
   out_2257575929542866463[32] = 0.0;
   out_2257575929542866463[33] = 0.0;
   out_2257575929542866463[34] = 0.0;
   out_2257575929542866463[35] = 0.0;
   out_2257575929542866463[36] = 0.0;
   out_2257575929542866463[37] = 0.0;
   out_2257575929542866463[38] = 0.0;
   out_2257575929542866463[39] = 0.0;
   out_2257575929542866463[40] = 1.0;
   out_2257575929542866463[41] = 0.0;
   out_2257575929542866463[42] = 0.0;
   out_2257575929542866463[43] = 0.0;
   out_2257575929542866463[44] = 0.0;
   out_2257575929542866463[45] = 0.0;
   out_2257575929542866463[46] = 0.0;
   out_2257575929542866463[47] = 0.0;
   out_2257575929542866463[48] = 0.0;
   out_2257575929542866463[49] = 0.0;
   out_2257575929542866463[50] = 1.0;
   out_2257575929542866463[51] = 0.0;
   out_2257575929542866463[52] = 0.0;
   out_2257575929542866463[53] = 0.0;
   out_2257575929542866463[54] = 0.0;
   out_2257575929542866463[55] = 0.0;
   out_2257575929542866463[56] = 0.0;
   out_2257575929542866463[57] = 0.0;
   out_2257575929542866463[58] = 0.0;
   out_2257575929542866463[59] = 0.0;
   out_2257575929542866463[60] = 1.0;
   out_2257575929542866463[61] = 0.0;
   out_2257575929542866463[62] = 0.0;
   out_2257575929542866463[63] = 0.0;
   out_2257575929542866463[64] = 0.0;
   out_2257575929542866463[65] = 0.0;
   out_2257575929542866463[66] = 0.0;
   out_2257575929542866463[67] = 0.0;
   out_2257575929542866463[68] = 0.0;
   out_2257575929542866463[69] = 0.0;
   out_2257575929542866463[70] = 1.0;
   out_2257575929542866463[71] = 0.0;
   out_2257575929542866463[72] = 0.0;
   out_2257575929542866463[73] = 0.0;
   out_2257575929542866463[74] = 0.0;
   out_2257575929542866463[75] = 0.0;
   out_2257575929542866463[76] = 0.0;
   out_2257575929542866463[77] = 0.0;
   out_2257575929542866463[78] = 0.0;
   out_2257575929542866463[79] = 0.0;
   out_2257575929542866463[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_798310912056979304) {
   out_798310912056979304[0] = state[0];
   out_798310912056979304[1] = state[1];
   out_798310912056979304[2] = state[2];
   out_798310912056979304[3] = state[3];
   out_798310912056979304[4] = state[4];
   out_798310912056979304[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_798310912056979304[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_798310912056979304[7] = state[7];
   out_798310912056979304[8] = state[8];
}
void F_fun(double *state, double dt, double *out_4628510486756966268) {
   out_4628510486756966268[0] = 1;
   out_4628510486756966268[1] = 0;
   out_4628510486756966268[2] = 0;
   out_4628510486756966268[3] = 0;
   out_4628510486756966268[4] = 0;
   out_4628510486756966268[5] = 0;
   out_4628510486756966268[6] = 0;
   out_4628510486756966268[7] = 0;
   out_4628510486756966268[8] = 0;
   out_4628510486756966268[9] = 0;
   out_4628510486756966268[10] = 1;
   out_4628510486756966268[11] = 0;
   out_4628510486756966268[12] = 0;
   out_4628510486756966268[13] = 0;
   out_4628510486756966268[14] = 0;
   out_4628510486756966268[15] = 0;
   out_4628510486756966268[16] = 0;
   out_4628510486756966268[17] = 0;
   out_4628510486756966268[18] = 0;
   out_4628510486756966268[19] = 0;
   out_4628510486756966268[20] = 1;
   out_4628510486756966268[21] = 0;
   out_4628510486756966268[22] = 0;
   out_4628510486756966268[23] = 0;
   out_4628510486756966268[24] = 0;
   out_4628510486756966268[25] = 0;
   out_4628510486756966268[26] = 0;
   out_4628510486756966268[27] = 0;
   out_4628510486756966268[28] = 0;
   out_4628510486756966268[29] = 0;
   out_4628510486756966268[30] = 1;
   out_4628510486756966268[31] = 0;
   out_4628510486756966268[32] = 0;
   out_4628510486756966268[33] = 0;
   out_4628510486756966268[34] = 0;
   out_4628510486756966268[35] = 0;
   out_4628510486756966268[36] = 0;
   out_4628510486756966268[37] = 0;
   out_4628510486756966268[38] = 0;
   out_4628510486756966268[39] = 0;
   out_4628510486756966268[40] = 1;
   out_4628510486756966268[41] = 0;
   out_4628510486756966268[42] = 0;
   out_4628510486756966268[43] = 0;
   out_4628510486756966268[44] = 0;
   out_4628510486756966268[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_4628510486756966268[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_4628510486756966268[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4628510486756966268[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4628510486756966268[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_4628510486756966268[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_4628510486756966268[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_4628510486756966268[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_4628510486756966268[53] = -9.8100000000000005*dt;
   out_4628510486756966268[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_4628510486756966268[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_4628510486756966268[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4628510486756966268[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4628510486756966268[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_4628510486756966268[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_4628510486756966268[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_4628510486756966268[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4628510486756966268[62] = 0;
   out_4628510486756966268[63] = 0;
   out_4628510486756966268[64] = 0;
   out_4628510486756966268[65] = 0;
   out_4628510486756966268[66] = 0;
   out_4628510486756966268[67] = 0;
   out_4628510486756966268[68] = 0;
   out_4628510486756966268[69] = 0;
   out_4628510486756966268[70] = 1;
   out_4628510486756966268[71] = 0;
   out_4628510486756966268[72] = 0;
   out_4628510486756966268[73] = 0;
   out_4628510486756966268[74] = 0;
   out_4628510486756966268[75] = 0;
   out_4628510486756966268[76] = 0;
   out_4628510486756966268[77] = 0;
   out_4628510486756966268[78] = 0;
   out_4628510486756966268[79] = 0;
   out_4628510486756966268[80] = 1;
}
void h_25(double *state, double *unused, double *out_3980621137363803839) {
   out_3980621137363803839[0] = state[6];
}
void H_25(double *state, double *unused, double *out_1308455007514384254) {
   out_1308455007514384254[0] = 0;
   out_1308455007514384254[1] = 0;
   out_1308455007514384254[2] = 0;
   out_1308455007514384254[3] = 0;
   out_1308455007514384254[4] = 0;
   out_1308455007514384254[5] = 0;
   out_1308455007514384254[6] = 1;
   out_1308455007514384254[7] = 0;
   out_1308455007514384254[8] = 0;
}
void h_24(double *state, double *unused, double *out_3882949458801228245) {
   out_3882949458801228245[0] = state[4];
   out_3882949458801228245[1] = state[5];
}
void H_24(double *state, double *unused, double *out_7919610178554810971) {
   out_7919610178554810971[0] = 0;
   out_7919610178554810971[1] = 0;
   out_7919610178554810971[2] = 0;
   out_7919610178554810971[3] = 0;
   out_7919610178554810971[4] = 1;
   out_7919610178554810971[5] = 0;
   out_7919610178554810971[6] = 0;
   out_7919610178554810971[7] = 0;
   out_7919610178554810971[8] = 0;
   out_7919610178554810971[9] = 0;
   out_7919610178554810971[10] = 0;
   out_7919610178554810971[11] = 0;
   out_7919610178554810971[12] = 0;
   out_7919610178554810971[13] = 0;
   out_7919610178554810971[14] = 1;
   out_7919610178554810971[15] = 0;
   out_7919610178554810971[16] = 0;
   out_7919610178554810971[17] = 0;
}
void h_30(double *state, double *unused, double *out_118937660935302187) {
   out_118937660935302187[0] = state[4];
}
void H_30(double *state, double *unused, double *out_5836151337641992452) {
   out_5836151337641992452[0] = 0;
   out_5836151337641992452[1] = 0;
   out_5836151337641992452[2] = 0;
   out_5836151337641992452[3] = 0;
   out_5836151337641992452[4] = 1;
   out_5836151337641992452[5] = 0;
   out_5836151337641992452[6] = 0;
   out_5836151337641992452[7] = 0;
   out_5836151337641992452[8] = 0;
}
void h_26(double *state, double *unused, double *out_2693614442334551196) {
   out_2693614442334551196[0] = state[7];
}
void H_26(double *state, double *unused, double *out_5049958326388440478) {
   out_5049958326388440478[0] = 0;
   out_5049958326388440478[1] = 0;
   out_5049958326388440478[2] = 0;
   out_5049958326388440478[3] = 0;
   out_5049958326388440478[4] = 0;
   out_5049958326388440478[5] = 0;
   out_5049958326388440478[6] = 0;
   out_5049958326388440478[7] = 1;
   out_5049958326388440478[8] = 0;
}
void h_27(double *state, double *unused, double *out_5039922275076672603) {
   out_5039922275076672603[0] = state[3];
}
void H_27(double *state, double *unused, double *out_3612557266458049235) {
   out_3612557266458049235[0] = 0;
   out_3612557266458049235[1] = 0;
   out_3612557266458049235[2] = 0;
   out_3612557266458049235[3] = 1;
   out_3612557266458049235[4] = 0;
   out_3612557266458049235[5] = 0;
   out_3612557266458049235[6] = 0;
   out_3612557266458049235[7] = 0;
   out_3612557266458049235[8] = 0;
}
void h_29(double *state, double *unused, double *out_8677371803330694953) {
   out_8677371803330694953[0] = state[1];
}
void H_29(double *state, double *unused, double *out_5325919993327600268) {
   out_5325919993327600268[0] = 0;
   out_5325919993327600268[1] = 1;
   out_5325919993327600268[2] = 0;
   out_5325919993327600268[3] = 0;
   out_5325919993327600268[4] = 0;
   out_5325919993327600268[5] = 0;
   out_5325919993327600268[6] = 0;
   out_5325919993327600268[7] = 0;
   out_5325919993327600268[8] = 0;
}
void h_28(double *state, double *unused, double *out_2003136223632339918) {
   out_2003136223632339918[0] = state[0];
}
void H_28(double *state, double *unused, double *out_3362289721762274017) {
   out_3362289721762274017[0] = 1;
   out_3362289721762274017[1] = 0;
   out_3362289721762274017[2] = 0;
   out_3362289721762274017[3] = 0;
   out_3362289721762274017[4] = 0;
   out_3362289721762274017[5] = 0;
   out_3362289721762274017[6] = 0;
   out_3362289721762274017[7] = 0;
   out_3362289721762274017[8] = 0;
}
void h_31(double *state, double *unused, double *out_6549578919060468205) {
   out_6549578919060468205[0] = state[8];
}
void H_31(double *state, double *unused, double *out_1277809045637423826) {
   out_1277809045637423826[0] = 0;
   out_1277809045637423826[1] = 0;
   out_1277809045637423826[2] = 0;
   out_1277809045637423826[3] = 0;
   out_1277809045637423826[4] = 0;
   out_1277809045637423826[5] = 0;
   out_1277809045637423826[6] = 0;
   out_1277809045637423826[7] = 0;
   out_1277809045637423826[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_7079130866779850545) {
  err_fun(nom_x, delta_x, out_7079130866779850545);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_6372496743972128365) {
  inv_err_fun(nom_x, true_x, out_6372496743972128365);
}
void car_H_mod_fun(double *state, double *out_2257575929542866463) {
  H_mod_fun(state, out_2257575929542866463);
}
void car_f_fun(double *state, double dt, double *out_798310912056979304) {
  f_fun(state,  dt, out_798310912056979304);
}
void car_F_fun(double *state, double dt, double *out_4628510486756966268) {
  F_fun(state,  dt, out_4628510486756966268);
}
void car_h_25(double *state, double *unused, double *out_3980621137363803839) {
  h_25(state, unused, out_3980621137363803839);
}
void car_H_25(double *state, double *unused, double *out_1308455007514384254) {
  H_25(state, unused, out_1308455007514384254);
}
void car_h_24(double *state, double *unused, double *out_3882949458801228245) {
  h_24(state, unused, out_3882949458801228245);
}
void car_H_24(double *state, double *unused, double *out_7919610178554810971) {
  H_24(state, unused, out_7919610178554810971);
}
void car_h_30(double *state, double *unused, double *out_118937660935302187) {
  h_30(state, unused, out_118937660935302187);
}
void car_H_30(double *state, double *unused, double *out_5836151337641992452) {
  H_30(state, unused, out_5836151337641992452);
}
void car_h_26(double *state, double *unused, double *out_2693614442334551196) {
  h_26(state, unused, out_2693614442334551196);
}
void car_H_26(double *state, double *unused, double *out_5049958326388440478) {
  H_26(state, unused, out_5049958326388440478);
}
void car_h_27(double *state, double *unused, double *out_5039922275076672603) {
  h_27(state, unused, out_5039922275076672603);
}
void car_H_27(double *state, double *unused, double *out_3612557266458049235) {
  H_27(state, unused, out_3612557266458049235);
}
void car_h_29(double *state, double *unused, double *out_8677371803330694953) {
  h_29(state, unused, out_8677371803330694953);
}
void car_H_29(double *state, double *unused, double *out_5325919993327600268) {
  H_29(state, unused, out_5325919993327600268);
}
void car_h_28(double *state, double *unused, double *out_2003136223632339918) {
  h_28(state, unused, out_2003136223632339918);
}
void car_H_28(double *state, double *unused, double *out_3362289721762274017) {
  H_28(state, unused, out_3362289721762274017);
}
void car_h_31(double *state, double *unused, double *out_6549578919060468205) {
  h_31(state, unused, out_6549578919060468205);
}
void car_H_31(double *state, double *unused, double *out_1277809045637423826) {
  H_31(state, unused, out_1277809045637423826);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
