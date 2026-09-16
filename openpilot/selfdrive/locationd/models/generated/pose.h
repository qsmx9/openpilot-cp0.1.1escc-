#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_8772440823350530000);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_2560461601569784888);
void pose_H_mod_fun(double *state, double *out_918935232433480461);
void pose_f_fun(double *state, double dt, double *out_8714094706690980781);
void pose_F_fun(double *state, double dt, double *out_6218200417019459507);
void pose_h_4(double *state, double *unused, double *out_5378180501345795798);
void pose_H_4(double *state, double *unused, double *out_2738633958339777322);
void pose_h_10(double *state, double *unused, double *out_2799911130635237718);
void pose_H_10(double *state, double *unused, double *out_704050404386079319);
void pose_h_13(double *state, double *unused, double *out_9150497396906774581);
void pose_H_13(double *state, double *unused, double *out_6572389421642301346);
void pose_h_14(double *state, double *unused, double *out_4533661798412794287);
void pose_H_14(double *state, double *unused, double *out_5821422390635149618);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}