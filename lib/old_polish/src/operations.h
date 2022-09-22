//
// Created by Gladis Ariane on 4/2/22.
//

#ifndef SRC_OPERATIONS_H_
#define SRC_OPERATIONS_H_
#define UNUSED __attribute__((unused))

#include <math.h>

double eval_add(double a, double b);

double eval_sub(double a, double b);  // x - y

double eval_mul(double a, double b);

double eval_div(double a, double b);

double eval_mod(double a, double b);

double eval_pow(double a, double b);

double eval_neg(double a, double b);  // -x

double eval_ctg(double x, UNUSED double u);

double eval_sin(double x, UNUSED double u);

double eval_cos(double x, UNUSED double u);

double eval_tan(double x, UNUSED double u);

double eval_asin(double x, UNUSED double u);

double eval_acos(double x, UNUSED double u);

double eval_atan(double x, UNUSED double u);

double eval_sqrt(double x, UNUSED double u);

double eval_log(double x, UNUSED double u);

double eval_ln(double x, UNUSED double u);

#endif  // SRC_OPERATIONS_H_
