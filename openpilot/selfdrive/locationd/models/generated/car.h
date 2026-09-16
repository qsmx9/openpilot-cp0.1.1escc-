#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_7079130866779850545);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_6372496743972128365);
void car_H_mod_fun(double *state, double *out_2257575929542866463);
void car_f_fun(double *state, double dt, double *out_798310912056979304);
void car_F_fun(double *state, double dt, double *out_4628510486756966268);
void car_h_25(double *state, double *unused, double *out_3980621137363803839);
void car_H_25(double *state, double *unused, double *out_1308455007514384254);
void car_h_24(double *state, double *unused, double *out_3882949458801228245);
void car_H_24(double *state, double *unused, double *out_7919610178554810971);
void car_h_30(double *state, double *unused, double *out_118937660935302187);
void car_H_30(double *state, double *unused, double *out_5836151337641992452);
void car_h_26(double *state, double *unused, double *out_2693614442334551196);
void car_H_26(double *state, double *unused, double *out_5049958326388440478);
void car_h_27(double *state, double *unused, double *out_5039922275076672603);
void car_H_27(double *state, double *unused, double *out_3612557266458049235);
void car_h_29(double *state, double *unused, double *out_8677371803330694953);
void car_H_29(double *state, double *unused, double *out_5325919993327600268);
void car_h_28(double *state, double *unused, double *out_2003136223632339918);
void car_H_28(double *state, double *unused, double *out_3362289721762274017);
void car_h_31(double *state, double *unused, double *out_6549578919060468205);
void car_H_31(double *state, double *unused, double *out_1277809045637423826);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}