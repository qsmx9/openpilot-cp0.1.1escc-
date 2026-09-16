#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_8772440823350530000) {
   out_8772440823350530000[0] = delta_x[0] + nom_x[0];
   out_8772440823350530000[1] = delta_x[1] + nom_x[1];
   out_8772440823350530000[2] = delta_x[2] + nom_x[2];
   out_8772440823350530000[3] = delta_x[3] + nom_x[3];
   out_8772440823350530000[4] = delta_x[4] + nom_x[4];
   out_8772440823350530000[5] = delta_x[5] + nom_x[5];
   out_8772440823350530000[6] = delta_x[6] + nom_x[6];
   out_8772440823350530000[7] = delta_x[7] + nom_x[7];
   out_8772440823350530000[8] = delta_x[8] + nom_x[8];
   out_8772440823350530000[9] = delta_x[9] + nom_x[9];
   out_8772440823350530000[10] = delta_x[10] + nom_x[10];
   out_8772440823350530000[11] = delta_x[11] + nom_x[11];
   out_8772440823350530000[12] = delta_x[12] + nom_x[12];
   out_8772440823350530000[13] = delta_x[13] + nom_x[13];
   out_8772440823350530000[14] = delta_x[14] + nom_x[14];
   out_8772440823350530000[15] = delta_x[15] + nom_x[15];
   out_8772440823350530000[16] = delta_x[16] + nom_x[16];
   out_8772440823350530000[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_2560461601569784888) {
   out_2560461601569784888[0] = -nom_x[0] + true_x[0];
   out_2560461601569784888[1] = -nom_x[1] + true_x[1];
   out_2560461601569784888[2] = -nom_x[2] + true_x[2];
   out_2560461601569784888[3] = -nom_x[3] + true_x[3];
   out_2560461601569784888[4] = -nom_x[4] + true_x[4];
   out_2560461601569784888[5] = -nom_x[5] + true_x[5];
   out_2560461601569784888[6] = -nom_x[6] + true_x[6];
   out_2560461601569784888[7] = -nom_x[7] + true_x[7];
   out_2560461601569784888[8] = -nom_x[8] + true_x[8];
   out_2560461601569784888[9] = -nom_x[9] + true_x[9];
   out_2560461601569784888[10] = -nom_x[10] + true_x[10];
   out_2560461601569784888[11] = -nom_x[11] + true_x[11];
   out_2560461601569784888[12] = -nom_x[12] + true_x[12];
   out_2560461601569784888[13] = -nom_x[13] + true_x[13];
   out_2560461601569784888[14] = -nom_x[14] + true_x[14];
   out_2560461601569784888[15] = -nom_x[15] + true_x[15];
   out_2560461601569784888[16] = -nom_x[16] + true_x[16];
   out_2560461601569784888[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_918935232433480461) {
   out_918935232433480461[0] = 1.0;
   out_918935232433480461[1] = 0.0;
   out_918935232433480461[2] = 0.0;
   out_918935232433480461[3] = 0.0;
   out_918935232433480461[4] = 0.0;
   out_918935232433480461[5] = 0.0;
   out_918935232433480461[6] = 0.0;
   out_918935232433480461[7] = 0.0;
   out_918935232433480461[8] = 0.0;
   out_918935232433480461[9] = 0.0;
   out_918935232433480461[10] = 0.0;
   out_918935232433480461[11] = 0.0;
   out_918935232433480461[12] = 0.0;
   out_918935232433480461[13] = 0.0;
   out_918935232433480461[14] = 0.0;
   out_918935232433480461[15] = 0.0;
   out_918935232433480461[16] = 0.0;
   out_918935232433480461[17] = 0.0;
   out_918935232433480461[18] = 0.0;
   out_918935232433480461[19] = 1.0;
   out_918935232433480461[20] = 0.0;
   out_918935232433480461[21] = 0.0;
   out_918935232433480461[22] = 0.0;
   out_918935232433480461[23] = 0.0;
   out_918935232433480461[24] = 0.0;
   out_918935232433480461[25] = 0.0;
   out_918935232433480461[26] = 0.0;
   out_918935232433480461[27] = 0.0;
   out_918935232433480461[28] = 0.0;
   out_918935232433480461[29] = 0.0;
   out_918935232433480461[30] = 0.0;
   out_918935232433480461[31] = 0.0;
   out_918935232433480461[32] = 0.0;
   out_918935232433480461[33] = 0.0;
   out_918935232433480461[34] = 0.0;
   out_918935232433480461[35] = 0.0;
   out_918935232433480461[36] = 0.0;
   out_918935232433480461[37] = 0.0;
   out_918935232433480461[38] = 1.0;
   out_918935232433480461[39] = 0.0;
   out_918935232433480461[40] = 0.0;
   out_918935232433480461[41] = 0.0;
   out_918935232433480461[42] = 0.0;
   out_918935232433480461[43] = 0.0;
   out_918935232433480461[44] = 0.0;
   out_918935232433480461[45] = 0.0;
   out_918935232433480461[46] = 0.0;
   out_918935232433480461[47] = 0.0;
   out_918935232433480461[48] = 0.0;
   out_918935232433480461[49] = 0.0;
   out_918935232433480461[50] = 0.0;
   out_918935232433480461[51] = 0.0;
   out_918935232433480461[52] = 0.0;
   out_918935232433480461[53] = 0.0;
   out_918935232433480461[54] = 0.0;
   out_918935232433480461[55] = 0.0;
   out_918935232433480461[56] = 0.0;
   out_918935232433480461[57] = 1.0;
   out_918935232433480461[58] = 0.0;
   out_918935232433480461[59] = 0.0;
   out_918935232433480461[60] = 0.0;
   out_918935232433480461[61] = 0.0;
   out_918935232433480461[62] = 0.0;
   out_918935232433480461[63] = 0.0;
   out_918935232433480461[64] = 0.0;
   out_918935232433480461[65] = 0.0;
   out_918935232433480461[66] = 0.0;
   out_918935232433480461[67] = 0.0;
   out_918935232433480461[68] = 0.0;
   out_918935232433480461[69] = 0.0;
   out_918935232433480461[70] = 0.0;
   out_918935232433480461[71] = 0.0;
   out_918935232433480461[72] = 0.0;
   out_918935232433480461[73] = 0.0;
   out_918935232433480461[74] = 0.0;
   out_918935232433480461[75] = 0.0;
   out_918935232433480461[76] = 1.0;
   out_918935232433480461[77] = 0.0;
   out_918935232433480461[78] = 0.0;
   out_918935232433480461[79] = 0.0;
   out_918935232433480461[80] = 0.0;
   out_918935232433480461[81] = 0.0;
   out_918935232433480461[82] = 0.0;
   out_918935232433480461[83] = 0.0;
   out_918935232433480461[84] = 0.0;
   out_918935232433480461[85] = 0.0;
   out_918935232433480461[86] = 0.0;
   out_918935232433480461[87] = 0.0;
   out_918935232433480461[88] = 0.0;
   out_918935232433480461[89] = 0.0;
   out_918935232433480461[90] = 0.0;
   out_918935232433480461[91] = 0.0;
   out_918935232433480461[92] = 0.0;
   out_918935232433480461[93] = 0.0;
   out_918935232433480461[94] = 0.0;
   out_918935232433480461[95] = 1.0;
   out_918935232433480461[96] = 0.0;
   out_918935232433480461[97] = 0.0;
   out_918935232433480461[98] = 0.0;
   out_918935232433480461[99] = 0.0;
   out_918935232433480461[100] = 0.0;
   out_918935232433480461[101] = 0.0;
   out_918935232433480461[102] = 0.0;
   out_918935232433480461[103] = 0.0;
   out_918935232433480461[104] = 0.0;
   out_918935232433480461[105] = 0.0;
   out_918935232433480461[106] = 0.0;
   out_918935232433480461[107] = 0.0;
   out_918935232433480461[108] = 0.0;
   out_918935232433480461[109] = 0.0;
   out_918935232433480461[110] = 0.0;
   out_918935232433480461[111] = 0.0;
   out_918935232433480461[112] = 0.0;
   out_918935232433480461[113] = 0.0;
   out_918935232433480461[114] = 1.0;
   out_918935232433480461[115] = 0.0;
   out_918935232433480461[116] = 0.0;
   out_918935232433480461[117] = 0.0;
   out_918935232433480461[118] = 0.0;
   out_918935232433480461[119] = 0.0;
   out_918935232433480461[120] = 0.0;
   out_918935232433480461[121] = 0.0;
   out_918935232433480461[122] = 0.0;
   out_918935232433480461[123] = 0.0;
   out_918935232433480461[124] = 0.0;
   out_918935232433480461[125] = 0.0;
   out_918935232433480461[126] = 0.0;
   out_918935232433480461[127] = 0.0;
   out_918935232433480461[128] = 0.0;
   out_918935232433480461[129] = 0.0;
   out_918935232433480461[130] = 0.0;
   out_918935232433480461[131] = 0.0;
   out_918935232433480461[132] = 0.0;
   out_918935232433480461[133] = 1.0;
   out_918935232433480461[134] = 0.0;
   out_918935232433480461[135] = 0.0;
   out_918935232433480461[136] = 0.0;
   out_918935232433480461[137] = 0.0;
   out_918935232433480461[138] = 0.0;
   out_918935232433480461[139] = 0.0;
   out_918935232433480461[140] = 0.0;
   out_918935232433480461[141] = 0.0;
   out_918935232433480461[142] = 0.0;
   out_918935232433480461[143] = 0.0;
   out_918935232433480461[144] = 0.0;
   out_918935232433480461[145] = 0.0;
   out_918935232433480461[146] = 0.0;
   out_918935232433480461[147] = 0.0;
   out_918935232433480461[148] = 0.0;
   out_918935232433480461[149] = 0.0;
   out_918935232433480461[150] = 0.0;
   out_918935232433480461[151] = 0.0;
   out_918935232433480461[152] = 1.0;
   out_918935232433480461[153] = 0.0;
   out_918935232433480461[154] = 0.0;
   out_918935232433480461[155] = 0.0;
   out_918935232433480461[156] = 0.0;
   out_918935232433480461[157] = 0.0;
   out_918935232433480461[158] = 0.0;
   out_918935232433480461[159] = 0.0;
   out_918935232433480461[160] = 0.0;
   out_918935232433480461[161] = 0.0;
   out_918935232433480461[162] = 0.0;
   out_918935232433480461[163] = 0.0;
   out_918935232433480461[164] = 0.0;
   out_918935232433480461[165] = 0.0;
   out_918935232433480461[166] = 0.0;
   out_918935232433480461[167] = 0.0;
   out_918935232433480461[168] = 0.0;
   out_918935232433480461[169] = 0.0;
   out_918935232433480461[170] = 0.0;
   out_918935232433480461[171] = 1.0;
   out_918935232433480461[172] = 0.0;
   out_918935232433480461[173] = 0.0;
   out_918935232433480461[174] = 0.0;
   out_918935232433480461[175] = 0.0;
   out_918935232433480461[176] = 0.0;
   out_918935232433480461[177] = 0.0;
   out_918935232433480461[178] = 0.0;
   out_918935232433480461[179] = 0.0;
   out_918935232433480461[180] = 0.0;
   out_918935232433480461[181] = 0.0;
   out_918935232433480461[182] = 0.0;
   out_918935232433480461[183] = 0.0;
   out_918935232433480461[184] = 0.0;
   out_918935232433480461[185] = 0.0;
   out_918935232433480461[186] = 0.0;
   out_918935232433480461[187] = 0.0;
   out_918935232433480461[188] = 0.0;
   out_918935232433480461[189] = 0.0;
   out_918935232433480461[190] = 1.0;
   out_918935232433480461[191] = 0.0;
   out_918935232433480461[192] = 0.0;
   out_918935232433480461[193] = 0.0;
   out_918935232433480461[194] = 0.0;
   out_918935232433480461[195] = 0.0;
   out_918935232433480461[196] = 0.0;
   out_918935232433480461[197] = 0.0;
   out_918935232433480461[198] = 0.0;
   out_918935232433480461[199] = 0.0;
   out_918935232433480461[200] = 0.0;
   out_918935232433480461[201] = 0.0;
   out_918935232433480461[202] = 0.0;
   out_918935232433480461[203] = 0.0;
   out_918935232433480461[204] = 0.0;
   out_918935232433480461[205] = 0.0;
   out_918935232433480461[206] = 0.0;
   out_918935232433480461[207] = 0.0;
   out_918935232433480461[208] = 0.0;
   out_918935232433480461[209] = 1.0;
   out_918935232433480461[210] = 0.0;
   out_918935232433480461[211] = 0.0;
   out_918935232433480461[212] = 0.0;
   out_918935232433480461[213] = 0.0;
   out_918935232433480461[214] = 0.0;
   out_918935232433480461[215] = 0.0;
   out_918935232433480461[216] = 0.0;
   out_918935232433480461[217] = 0.0;
   out_918935232433480461[218] = 0.0;
   out_918935232433480461[219] = 0.0;
   out_918935232433480461[220] = 0.0;
   out_918935232433480461[221] = 0.0;
   out_918935232433480461[222] = 0.0;
   out_918935232433480461[223] = 0.0;
   out_918935232433480461[224] = 0.0;
   out_918935232433480461[225] = 0.0;
   out_918935232433480461[226] = 0.0;
   out_918935232433480461[227] = 0.0;
   out_918935232433480461[228] = 1.0;
   out_918935232433480461[229] = 0.0;
   out_918935232433480461[230] = 0.0;
   out_918935232433480461[231] = 0.0;
   out_918935232433480461[232] = 0.0;
   out_918935232433480461[233] = 0.0;
   out_918935232433480461[234] = 0.0;
   out_918935232433480461[235] = 0.0;
   out_918935232433480461[236] = 0.0;
   out_918935232433480461[237] = 0.0;
   out_918935232433480461[238] = 0.0;
   out_918935232433480461[239] = 0.0;
   out_918935232433480461[240] = 0.0;
   out_918935232433480461[241] = 0.0;
   out_918935232433480461[242] = 0.0;
   out_918935232433480461[243] = 0.0;
   out_918935232433480461[244] = 0.0;
   out_918935232433480461[245] = 0.0;
   out_918935232433480461[246] = 0.0;
   out_918935232433480461[247] = 1.0;
   out_918935232433480461[248] = 0.0;
   out_918935232433480461[249] = 0.0;
   out_918935232433480461[250] = 0.0;
   out_918935232433480461[251] = 0.0;
   out_918935232433480461[252] = 0.0;
   out_918935232433480461[253] = 0.0;
   out_918935232433480461[254] = 0.0;
   out_918935232433480461[255] = 0.0;
   out_918935232433480461[256] = 0.0;
   out_918935232433480461[257] = 0.0;
   out_918935232433480461[258] = 0.0;
   out_918935232433480461[259] = 0.0;
   out_918935232433480461[260] = 0.0;
   out_918935232433480461[261] = 0.0;
   out_918935232433480461[262] = 0.0;
   out_918935232433480461[263] = 0.0;
   out_918935232433480461[264] = 0.0;
   out_918935232433480461[265] = 0.0;
   out_918935232433480461[266] = 1.0;
   out_918935232433480461[267] = 0.0;
   out_918935232433480461[268] = 0.0;
   out_918935232433480461[269] = 0.0;
   out_918935232433480461[270] = 0.0;
   out_918935232433480461[271] = 0.0;
   out_918935232433480461[272] = 0.0;
   out_918935232433480461[273] = 0.0;
   out_918935232433480461[274] = 0.0;
   out_918935232433480461[275] = 0.0;
   out_918935232433480461[276] = 0.0;
   out_918935232433480461[277] = 0.0;
   out_918935232433480461[278] = 0.0;
   out_918935232433480461[279] = 0.0;
   out_918935232433480461[280] = 0.0;
   out_918935232433480461[281] = 0.0;
   out_918935232433480461[282] = 0.0;
   out_918935232433480461[283] = 0.0;
   out_918935232433480461[284] = 0.0;
   out_918935232433480461[285] = 1.0;
   out_918935232433480461[286] = 0.0;
   out_918935232433480461[287] = 0.0;
   out_918935232433480461[288] = 0.0;
   out_918935232433480461[289] = 0.0;
   out_918935232433480461[290] = 0.0;
   out_918935232433480461[291] = 0.0;
   out_918935232433480461[292] = 0.0;
   out_918935232433480461[293] = 0.0;
   out_918935232433480461[294] = 0.0;
   out_918935232433480461[295] = 0.0;
   out_918935232433480461[296] = 0.0;
   out_918935232433480461[297] = 0.0;
   out_918935232433480461[298] = 0.0;
   out_918935232433480461[299] = 0.0;
   out_918935232433480461[300] = 0.0;
   out_918935232433480461[301] = 0.0;
   out_918935232433480461[302] = 0.0;
   out_918935232433480461[303] = 0.0;
   out_918935232433480461[304] = 1.0;
   out_918935232433480461[305] = 0.0;
   out_918935232433480461[306] = 0.0;
   out_918935232433480461[307] = 0.0;
   out_918935232433480461[308] = 0.0;
   out_918935232433480461[309] = 0.0;
   out_918935232433480461[310] = 0.0;
   out_918935232433480461[311] = 0.0;
   out_918935232433480461[312] = 0.0;
   out_918935232433480461[313] = 0.0;
   out_918935232433480461[314] = 0.0;
   out_918935232433480461[315] = 0.0;
   out_918935232433480461[316] = 0.0;
   out_918935232433480461[317] = 0.0;
   out_918935232433480461[318] = 0.0;
   out_918935232433480461[319] = 0.0;
   out_918935232433480461[320] = 0.0;
   out_918935232433480461[321] = 0.0;
   out_918935232433480461[322] = 0.0;
   out_918935232433480461[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_8714094706690980781) {
   out_8714094706690980781[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_8714094706690980781[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_8714094706690980781[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_8714094706690980781[3] = dt*state[12] + state[3];
   out_8714094706690980781[4] = dt*state[13] + state[4];
   out_8714094706690980781[5] = dt*state[14] + state[5];
   out_8714094706690980781[6] = state[6];
   out_8714094706690980781[7] = state[7];
   out_8714094706690980781[8] = state[8];
   out_8714094706690980781[9] = state[9];
   out_8714094706690980781[10] = state[10];
   out_8714094706690980781[11] = state[11];
   out_8714094706690980781[12] = state[12];
   out_8714094706690980781[13] = state[13];
   out_8714094706690980781[14] = state[14];
   out_8714094706690980781[15] = state[15];
   out_8714094706690980781[16] = state[16];
   out_8714094706690980781[17] = state[17];
}
void F_fun(double *state, double dt, double *out_6218200417019459507) {
   out_6218200417019459507[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6218200417019459507[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6218200417019459507[2] = 0;
   out_6218200417019459507[3] = 0;
   out_6218200417019459507[4] = 0;
   out_6218200417019459507[5] = 0;
   out_6218200417019459507[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6218200417019459507[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6218200417019459507[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6218200417019459507[9] = 0;
   out_6218200417019459507[10] = 0;
   out_6218200417019459507[11] = 0;
   out_6218200417019459507[12] = 0;
   out_6218200417019459507[13] = 0;
   out_6218200417019459507[14] = 0;
   out_6218200417019459507[15] = 0;
   out_6218200417019459507[16] = 0;
   out_6218200417019459507[17] = 0;
   out_6218200417019459507[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6218200417019459507[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6218200417019459507[20] = 0;
   out_6218200417019459507[21] = 0;
   out_6218200417019459507[22] = 0;
   out_6218200417019459507[23] = 0;
   out_6218200417019459507[24] = 0;
   out_6218200417019459507[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6218200417019459507[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6218200417019459507[27] = 0;
   out_6218200417019459507[28] = 0;
   out_6218200417019459507[29] = 0;
   out_6218200417019459507[30] = 0;
   out_6218200417019459507[31] = 0;
   out_6218200417019459507[32] = 0;
   out_6218200417019459507[33] = 0;
   out_6218200417019459507[34] = 0;
   out_6218200417019459507[35] = 0;
   out_6218200417019459507[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6218200417019459507[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6218200417019459507[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6218200417019459507[39] = 0;
   out_6218200417019459507[40] = 0;
   out_6218200417019459507[41] = 0;
   out_6218200417019459507[42] = 0;
   out_6218200417019459507[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6218200417019459507[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6218200417019459507[45] = 0;
   out_6218200417019459507[46] = 0;
   out_6218200417019459507[47] = 0;
   out_6218200417019459507[48] = 0;
   out_6218200417019459507[49] = 0;
   out_6218200417019459507[50] = 0;
   out_6218200417019459507[51] = 0;
   out_6218200417019459507[52] = 0;
   out_6218200417019459507[53] = 0;
   out_6218200417019459507[54] = 0;
   out_6218200417019459507[55] = 0;
   out_6218200417019459507[56] = 0;
   out_6218200417019459507[57] = 1;
   out_6218200417019459507[58] = 0;
   out_6218200417019459507[59] = 0;
   out_6218200417019459507[60] = 0;
   out_6218200417019459507[61] = 0;
   out_6218200417019459507[62] = 0;
   out_6218200417019459507[63] = 0;
   out_6218200417019459507[64] = 0;
   out_6218200417019459507[65] = 0;
   out_6218200417019459507[66] = dt;
   out_6218200417019459507[67] = 0;
   out_6218200417019459507[68] = 0;
   out_6218200417019459507[69] = 0;
   out_6218200417019459507[70] = 0;
   out_6218200417019459507[71] = 0;
   out_6218200417019459507[72] = 0;
   out_6218200417019459507[73] = 0;
   out_6218200417019459507[74] = 0;
   out_6218200417019459507[75] = 0;
   out_6218200417019459507[76] = 1;
   out_6218200417019459507[77] = 0;
   out_6218200417019459507[78] = 0;
   out_6218200417019459507[79] = 0;
   out_6218200417019459507[80] = 0;
   out_6218200417019459507[81] = 0;
   out_6218200417019459507[82] = 0;
   out_6218200417019459507[83] = 0;
   out_6218200417019459507[84] = 0;
   out_6218200417019459507[85] = dt;
   out_6218200417019459507[86] = 0;
   out_6218200417019459507[87] = 0;
   out_6218200417019459507[88] = 0;
   out_6218200417019459507[89] = 0;
   out_6218200417019459507[90] = 0;
   out_6218200417019459507[91] = 0;
   out_6218200417019459507[92] = 0;
   out_6218200417019459507[93] = 0;
   out_6218200417019459507[94] = 0;
   out_6218200417019459507[95] = 1;
   out_6218200417019459507[96] = 0;
   out_6218200417019459507[97] = 0;
   out_6218200417019459507[98] = 0;
   out_6218200417019459507[99] = 0;
   out_6218200417019459507[100] = 0;
   out_6218200417019459507[101] = 0;
   out_6218200417019459507[102] = 0;
   out_6218200417019459507[103] = 0;
   out_6218200417019459507[104] = dt;
   out_6218200417019459507[105] = 0;
   out_6218200417019459507[106] = 0;
   out_6218200417019459507[107] = 0;
   out_6218200417019459507[108] = 0;
   out_6218200417019459507[109] = 0;
   out_6218200417019459507[110] = 0;
   out_6218200417019459507[111] = 0;
   out_6218200417019459507[112] = 0;
   out_6218200417019459507[113] = 0;
   out_6218200417019459507[114] = 1;
   out_6218200417019459507[115] = 0;
   out_6218200417019459507[116] = 0;
   out_6218200417019459507[117] = 0;
   out_6218200417019459507[118] = 0;
   out_6218200417019459507[119] = 0;
   out_6218200417019459507[120] = 0;
   out_6218200417019459507[121] = 0;
   out_6218200417019459507[122] = 0;
   out_6218200417019459507[123] = 0;
   out_6218200417019459507[124] = 0;
   out_6218200417019459507[125] = 0;
   out_6218200417019459507[126] = 0;
   out_6218200417019459507[127] = 0;
   out_6218200417019459507[128] = 0;
   out_6218200417019459507[129] = 0;
   out_6218200417019459507[130] = 0;
   out_6218200417019459507[131] = 0;
   out_6218200417019459507[132] = 0;
   out_6218200417019459507[133] = 1;
   out_6218200417019459507[134] = 0;
   out_6218200417019459507[135] = 0;
   out_6218200417019459507[136] = 0;
   out_6218200417019459507[137] = 0;
   out_6218200417019459507[138] = 0;
   out_6218200417019459507[139] = 0;
   out_6218200417019459507[140] = 0;
   out_6218200417019459507[141] = 0;
   out_6218200417019459507[142] = 0;
   out_6218200417019459507[143] = 0;
   out_6218200417019459507[144] = 0;
   out_6218200417019459507[145] = 0;
   out_6218200417019459507[146] = 0;
   out_6218200417019459507[147] = 0;
   out_6218200417019459507[148] = 0;
   out_6218200417019459507[149] = 0;
   out_6218200417019459507[150] = 0;
   out_6218200417019459507[151] = 0;
   out_6218200417019459507[152] = 1;
   out_6218200417019459507[153] = 0;
   out_6218200417019459507[154] = 0;
   out_6218200417019459507[155] = 0;
   out_6218200417019459507[156] = 0;
   out_6218200417019459507[157] = 0;
   out_6218200417019459507[158] = 0;
   out_6218200417019459507[159] = 0;
   out_6218200417019459507[160] = 0;
   out_6218200417019459507[161] = 0;
   out_6218200417019459507[162] = 0;
   out_6218200417019459507[163] = 0;
   out_6218200417019459507[164] = 0;
   out_6218200417019459507[165] = 0;
   out_6218200417019459507[166] = 0;
   out_6218200417019459507[167] = 0;
   out_6218200417019459507[168] = 0;
   out_6218200417019459507[169] = 0;
   out_6218200417019459507[170] = 0;
   out_6218200417019459507[171] = 1;
   out_6218200417019459507[172] = 0;
   out_6218200417019459507[173] = 0;
   out_6218200417019459507[174] = 0;
   out_6218200417019459507[175] = 0;
   out_6218200417019459507[176] = 0;
   out_6218200417019459507[177] = 0;
   out_6218200417019459507[178] = 0;
   out_6218200417019459507[179] = 0;
   out_6218200417019459507[180] = 0;
   out_6218200417019459507[181] = 0;
   out_6218200417019459507[182] = 0;
   out_6218200417019459507[183] = 0;
   out_6218200417019459507[184] = 0;
   out_6218200417019459507[185] = 0;
   out_6218200417019459507[186] = 0;
   out_6218200417019459507[187] = 0;
   out_6218200417019459507[188] = 0;
   out_6218200417019459507[189] = 0;
   out_6218200417019459507[190] = 1;
   out_6218200417019459507[191] = 0;
   out_6218200417019459507[192] = 0;
   out_6218200417019459507[193] = 0;
   out_6218200417019459507[194] = 0;
   out_6218200417019459507[195] = 0;
   out_6218200417019459507[196] = 0;
   out_6218200417019459507[197] = 0;
   out_6218200417019459507[198] = 0;
   out_6218200417019459507[199] = 0;
   out_6218200417019459507[200] = 0;
   out_6218200417019459507[201] = 0;
   out_6218200417019459507[202] = 0;
   out_6218200417019459507[203] = 0;
   out_6218200417019459507[204] = 0;
   out_6218200417019459507[205] = 0;
   out_6218200417019459507[206] = 0;
   out_6218200417019459507[207] = 0;
   out_6218200417019459507[208] = 0;
   out_6218200417019459507[209] = 1;
   out_6218200417019459507[210] = 0;
   out_6218200417019459507[211] = 0;
   out_6218200417019459507[212] = 0;
   out_6218200417019459507[213] = 0;
   out_6218200417019459507[214] = 0;
   out_6218200417019459507[215] = 0;
   out_6218200417019459507[216] = 0;
   out_6218200417019459507[217] = 0;
   out_6218200417019459507[218] = 0;
   out_6218200417019459507[219] = 0;
   out_6218200417019459507[220] = 0;
   out_6218200417019459507[221] = 0;
   out_6218200417019459507[222] = 0;
   out_6218200417019459507[223] = 0;
   out_6218200417019459507[224] = 0;
   out_6218200417019459507[225] = 0;
   out_6218200417019459507[226] = 0;
   out_6218200417019459507[227] = 0;
   out_6218200417019459507[228] = 1;
   out_6218200417019459507[229] = 0;
   out_6218200417019459507[230] = 0;
   out_6218200417019459507[231] = 0;
   out_6218200417019459507[232] = 0;
   out_6218200417019459507[233] = 0;
   out_6218200417019459507[234] = 0;
   out_6218200417019459507[235] = 0;
   out_6218200417019459507[236] = 0;
   out_6218200417019459507[237] = 0;
   out_6218200417019459507[238] = 0;
   out_6218200417019459507[239] = 0;
   out_6218200417019459507[240] = 0;
   out_6218200417019459507[241] = 0;
   out_6218200417019459507[242] = 0;
   out_6218200417019459507[243] = 0;
   out_6218200417019459507[244] = 0;
   out_6218200417019459507[245] = 0;
   out_6218200417019459507[246] = 0;
   out_6218200417019459507[247] = 1;
   out_6218200417019459507[248] = 0;
   out_6218200417019459507[249] = 0;
   out_6218200417019459507[250] = 0;
   out_6218200417019459507[251] = 0;
   out_6218200417019459507[252] = 0;
   out_6218200417019459507[253] = 0;
   out_6218200417019459507[254] = 0;
   out_6218200417019459507[255] = 0;
   out_6218200417019459507[256] = 0;
   out_6218200417019459507[257] = 0;
   out_6218200417019459507[258] = 0;
   out_6218200417019459507[259] = 0;
   out_6218200417019459507[260] = 0;
   out_6218200417019459507[261] = 0;
   out_6218200417019459507[262] = 0;
   out_6218200417019459507[263] = 0;
   out_6218200417019459507[264] = 0;
   out_6218200417019459507[265] = 0;
   out_6218200417019459507[266] = 1;
   out_6218200417019459507[267] = 0;
   out_6218200417019459507[268] = 0;
   out_6218200417019459507[269] = 0;
   out_6218200417019459507[270] = 0;
   out_6218200417019459507[271] = 0;
   out_6218200417019459507[272] = 0;
   out_6218200417019459507[273] = 0;
   out_6218200417019459507[274] = 0;
   out_6218200417019459507[275] = 0;
   out_6218200417019459507[276] = 0;
   out_6218200417019459507[277] = 0;
   out_6218200417019459507[278] = 0;
   out_6218200417019459507[279] = 0;
   out_6218200417019459507[280] = 0;
   out_6218200417019459507[281] = 0;
   out_6218200417019459507[282] = 0;
   out_6218200417019459507[283] = 0;
   out_6218200417019459507[284] = 0;
   out_6218200417019459507[285] = 1;
   out_6218200417019459507[286] = 0;
   out_6218200417019459507[287] = 0;
   out_6218200417019459507[288] = 0;
   out_6218200417019459507[289] = 0;
   out_6218200417019459507[290] = 0;
   out_6218200417019459507[291] = 0;
   out_6218200417019459507[292] = 0;
   out_6218200417019459507[293] = 0;
   out_6218200417019459507[294] = 0;
   out_6218200417019459507[295] = 0;
   out_6218200417019459507[296] = 0;
   out_6218200417019459507[297] = 0;
   out_6218200417019459507[298] = 0;
   out_6218200417019459507[299] = 0;
   out_6218200417019459507[300] = 0;
   out_6218200417019459507[301] = 0;
   out_6218200417019459507[302] = 0;
   out_6218200417019459507[303] = 0;
   out_6218200417019459507[304] = 1;
   out_6218200417019459507[305] = 0;
   out_6218200417019459507[306] = 0;
   out_6218200417019459507[307] = 0;
   out_6218200417019459507[308] = 0;
   out_6218200417019459507[309] = 0;
   out_6218200417019459507[310] = 0;
   out_6218200417019459507[311] = 0;
   out_6218200417019459507[312] = 0;
   out_6218200417019459507[313] = 0;
   out_6218200417019459507[314] = 0;
   out_6218200417019459507[315] = 0;
   out_6218200417019459507[316] = 0;
   out_6218200417019459507[317] = 0;
   out_6218200417019459507[318] = 0;
   out_6218200417019459507[319] = 0;
   out_6218200417019459507[320] = 0;
   out_6218200417019459507[321] = 0;
   out_6218200417019459507[322] = 0;
   out_6218200417019459507[323] = 1;
}
void h_4(double *state, double *unused, double *out_5378180501345795798) {
   out_5378180501345795798[0] = state[6] + state[9];
   out_5378180501345795798[1] = state[7] + state[10];
   out_5378180501345795798[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_2738633958339777322) {
   out_2738633958339777322[0] = 0;
   out_2738633958339777322[1] = 0;
   out_2738633958339777322[2] = 0;
   out_2738633958339777322[3] = 0;
   out_2738633958339777322[4] = 0;
   out_2738633958339777322[5] = 0;
   out_2738633958339777322[6] = 1;
   out_2738633958339777322[7] = 0;
   out_2738633958339777322[8] = 0;
   out_2738633958339777322[9] = 1;
   out_2738633958339777322[10] = 0;
   out_2738633958339777322[11] = 0;
   out_2738633958339777322[12] = 0;
   out_2738633958339777322[13] = 0;
   out_2738633958339777322[14] = 0;
   out_2738633958339777322[15] = 0;
   out_2738633958339777322[16] = 0;
   out_2738633958339777322[17] = 0;
   out_2738633958339777322[18] = 0;
   out_2738633958339777322[19] = 0;
   out_2738633958339777322[20] = 0;
   out_2738633958339777322[21] = 0;
   out_2738633958339777322[22] = 0;
   out_2738633958339777322[23] = 0;
   out_2738633958339777322[24] = 0;
   out_2738633958339777322[25] = 1;
   out_2738633958339777322[26] = 0;
   out_2738633958339777322[27] = 0;
   out_2738633958339777322[28] = 1;
   out_2738633958339777322[29] = 0;
   out_2738633958339777322[30] = 0;
   out_2738633958339777322[31] = 0;
   out_2738633958339777322[32] = 0;
   out_2738633958339777322[33] = 0;
   out_2738633958339777322[34] = 0;
   out_2738633958339777322[35] = 0;
   out_2738633958339777322[36] = 0;
   out_2738633958339777322[37] = 0;
   out_2738633958339777322[38] = 0;
   out_2738633958339777322[39] = 0;
   out_2738633958339777322[40] = 0;
   out_2738633958339777322[41] = 0;
   out_2738633958339777322[42] = 0;
   out_2738633958339777322[43] = 0;
   out_2738633958339777322[44] = 1;
   out_2738633958339777322[45] = 0;
   out_2738633958339777322[46] = 0;
   out_2738633958339777322[47] = 1;
   out_2738633958339777322[48] = 0;
   out_2738633958339777322[49] = 0;
   out_2738633958339777322[50] = 0;
   out_2738633958339777322[51] = 0;
   out_2738633958339777322[52] = 0;
   out_2738633958339777322[53] = 0;
}
void h_10(double *state, double *unused, double *out_2799911130635237718) {
   out_2799911130635237718[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_2799911130635237718[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_2799911130635237718[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_704050404386079319) {
   out_704050404386079319[0] = 0;
   out_704050404386079319[1] = 9.8100000000000005*cos(state[1]);
   out_704050404386079319[2] = 0;
   out_704050404386079319[3] = 0;
   out_704050404386079319[4] = -state[8];
   out_704050404386079319[5] = state[7];
   out_704050404386079319[6] = 0;
   out_704050404386079319[7] = state[5];
   out_704050404386079319[8] = -state[4];
   out_704050404386079319[9] = 0;
   out_704050404386079319[10] = 0;
   out_704050404386079319[11] = 0;
   out_704050404386079319[12] = 1;
   out_704050404386079319[13] = 0;
   out_704050404386079319[14] = 0;
   out_704050404386079319[15] = 1;
   out_704050404386079319[16] = 0;
   out_704050404386079319[17] = 0;
   out_704050404386079319[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_704050404386079319[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_704050404386079319[20] = 0;
   out_704050404386079319[21] = state[8];
   out_704050404386079319[22] = 0;
   out_704050404386079319[23] = -state[6];
   out_704050404386079319[24] = -state[5];
   out_704050404386079319[25] = 0;
   out_704050404386079319[26] = state[3];
   out_704050404386079319[27] = 0;
   out_704050404386079319[28] = 0;
   out_704050404386079319[29] = 0;
   out_704050404386079319[30] = 0;
   out_704050404386079319[31] = 1;
   out_704050404386079319[32] = 0;
   out_704050404386079319[33] = 0;
   out_704050404386079319[34] = 1;
   out_704050404386079319[35] = 0;
   out_704050404386079319[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_704050404386079319[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_704050404386079319[38] = 0;
   out_704050404386079319[39] = -state[7];
   out_704050404386079319[40] = state[6];
   out_704050404386079319[41] = 0;
   out_704050404386079319[42] = state[4];
   out_704050404386079319[43] = -state[3];
   out_704050404386079319[44] = 0;
   out_704050404386079319[45] = 0;
   out_704050404386079319[46] = 0;
   out_704050404386079319[47] = 0;
   out_704050404386079319[48] = 0;
   out_704050404386079319[49] = 0;
   out_704050404386079319[50] = 1;
   out_704050404386079319[51] = 0;
   out_704050404386079319[52] = 0;
   out_704050404386079319[53] = 1;
}
void h_13(double *state, double *unused, double *out_9150497396906774581) {
   out_9150497396906774581[0] = state[3];
   out_9150497396906774581[1] = state[4];
   out_9150497396906774581[2] = state[5];
}
void H_13(double *state, double *unused, double *out_6572389421642301346) {
   out_6572389421642301346[0] = 0;
   out_6572389421642301346[1] = 0;
   out_6572389421642301346[2] = 0;
   out_6572389421642301346[3] = 1;
   out_6572389421642301346[4] = 0;
   out_6572389421642301346[5] = 0;
   out_6572389421642301346[6] = 0;
   out_6572389421642301346[7] = 0;
   out_6572389421642301346[8] = 0;
   out_6572389421642301346[9] = 0;
   out_6572389421642301346[10] = 0;
   out_6572389421642301346[11] = 0;
   out_6572389421642301346[12] = 0;
   out_6572389421642301346[13] = 0;
   out_6572389421642301346[14] = 0;
   out_6572389421642301346[15] = 0;
   out_6572389421642301346[16] = 0;
   out_6572389421642301346[17] = 0;
   out_6572389421642301346[18] = 0;
   out_6572389421642301346[19] = 0;
   out_6572389421642301346[20] = 0;
   out_6572389421642301346[21] = 0;
   out_6572389421642301346[22] = 1;
   out_6572389421642301346[23] = 0;
   out_6572389421642301346[24] = 0;
   out_6572389421642301346[25] = 0;
   out_6572389421642301346[26] = 0;
   out_6572389421642301346[27] = 0;
   out_6572389421642301346[28] = 0;
   out_6572389421642301346[29] = 0;
   out_6572389421642301346[30] = 0;
   out_6572389421642301346[31] = 0;
   out_6572389421642301346[32] = 0;
   out_6572389421642301346[33] = 0;
   out_6572389421642301346[34] = 0;
   out_6572389421642301346[35] = 0;
   out_6572389421642301346[36] = 0;
   out_6572389421642301346[37] = 0;
   out_6572389421642301346[38] = 0;
   out_6572389421642301346[39] = 0;
   out_6572389421642301346[40] = 0;
   out_6572389421642301346[41] = 1;
   out_6572389421642301346[42] = 0;
   out_6572389421642301346[43] = 0;
   out_6572389421642301346[44] = 0;
   out_6572389421642301346[45] = 0;
   out_6572389421642301346[46] = 0;
   out_6572389421642301346[47] = 0;
   out_6572389421642301346[48] = 0;
   out_6572389421642301346[49] = 0;
   out_6572389421642301346[50] = 0;
   out_6572389421642301346[51] = 0;
   out_6572389421642301346[52] = 0;
   out_6572389421642301346[53] = 0;
}
void h_14(double *state, double *unused, double *out_4533661798412794287) {
   out_4533661798412794287[0] = state[6];
   out_4533661798412794287[1] = state[7];
   out_4533661798412794287[2] = state[8];
}
void H_14(double *state, double *unused, double *out_5821422390635149618) {
   out_5821422390635149618[0] = 0;
   out_5821422390635149618[1] = 0;
   out_5821422390635149618[2] = 0;
   out_5821422390635149618[3] = 0;
   out_5821422390635149618[4] = 0;
   out_5821422390635149618[5] = 0;
   out_5821422390635149618[6] = 1;
   out_5821422390635149618[7] = 0;
   out_5821422390635149618[8] = 0;
   out_5821422390635149618[9] = 0;
   out_5821422390635149618[10] = 0;
   out_5821422390635149618[11] = 0;
   out_5821422390635149618[12] = 0;
   out_5821422390635149618[13] = 0;
   out_5821422390635149618[14] = 0;
   out_5821422390635149618[15] = 0;
   out_5821422390635149618[16] = 0;
   out_5821422390635149618[17] = 0;
   out_5821422390635149618[18] = 0;
   out_5821422390635149618[19] = 0;
   out_5821422390635149618[20] = 0;
   out_5821422390635149618[21] = 0;
   out_5821422390635149618[22] = 0;
   out_5821422390635149618[23] = 0;
   out_5821422390635149618[24] = 0;
   out_5821422390635149618[25] = 1;
   out_5821422390635149618[26] = 0;
   out_5821422390635149618[27] = 0;
   out_5821422390635149618[28] = 0;
   out_5821422390635149618[29] = 0;
   out_5821422390635149618[30] = 0;
   out_5821422390635149618[31] = 0;
   out_5821422390635149618[32] = 0;
   out_5821422390635149618[33] = 0;
   out_5821422390635149618[34] = 0;
   out_5821422390635149618[35] = 0;
   out_5821422390635149618[36] = 0;
   out_5821422390635149618[37] = 0;
   out_5821422390635149618[38] = 0;
   out_5821422390635149618[39] = 0;
   out_5821422390635149618[40] = 0;
   out_5821422390635149618[41] = 0;
   out_5821422390635149618[42] = 0;
   out_5821422390635149618[43] = 0;
   out_5821422390635149618[44] = 1;
   out_5821422390635149618[45] = 0;
   out_5821422390635149618[46] = 0;
   out_5821422390635149618[47] = 0;
   out_5821422390635149618[48] = 0;
   out_5821422390635149618[49] = 0;
   out_5821422390635149618[50] = 0;
   out_5821422390635149618[51] = 0;
   out_5821422390635149618[52] = 0;
   out_5821422390635149618[53] = 0;
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

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_8772440823350530000) {
  err_fun(nom_x, delta_x, out_8772440823350530000);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_2560461601569784888) {
  inv_err_fun(nom_x, true_x, out_2560461601569784888);
}
void pose_H_mod_fun(double *state, double *out_918935232433480461) {
  H_mod_fun(state, out_918935232433480461);
}
void pose_f_fun(double *state, double dt, double *out_8714094706690980781) {
  f_fun(state,  dt, out_8714094706690980781);
}
void pose_F_fun(double *state, double dt, double *out_6218200417019459507) {
  F_fun(state,  dt, out_6218200417019459507);
}
void pose_h_4(double *state, double *unused, double *out_5378180501345795798) {
  h_4(state, unused, out_5378180501345795798);
}
void pose_H_4(double *state, double *unused, double *out_2738633958339777322) {
  H_4(state, unused, out_2738633958339777322);
}
void pose_h_10(double *state, double *unused, double *out_2799911130635237718) {
  h_10(state, unused, out_2799911130635237718);
}
void pose_H_10(double *state, double *unused, double *out_704050404386079319) {
  H_10(state, unused, out_704050404386079319);
}
void pose_h_13(double *state, double *unused, double *out_9150497396906774581) {
  h_13(state, unused, out_9150497396906774581);
}
void pose_H_13(double *state, double *unused, double *out_6572389421642301346) {
  H_13(state, unused, out_6572389421642301346);
}
void pose_h_14(double *state, double *unused, double *out_4533661798412794287) {
  h_14(state, unused, out_4533661798412794287);
}
void pose_H_14(double *state, double *unused, double *out_5821422390635149618) {
  H_14(state, unused, out_5821422390635149618);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
