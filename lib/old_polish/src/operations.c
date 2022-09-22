//
// Created by Gladis Ariane on 4/2/22.
//

#include "operations.h"

double eval_add(double a, double b) { return a + b; }

double eval_sub(double a, double b) { return a - b; }

double eval_mul(double a, double b) { return a * b; }

double eval_div(double a, double b) { return a / b; }

double eval_mod(double a, double b) { return fmod(a, b); }

double eval_pow(double a, double b) { return pow(a, b); }

double eval_neg(double a, UNUSED double b) { return -a; }

double eval_sin(double x, UNUSED double u) { return sin(x); }

double eval_cos(double x, UNUSED double u) { return cos(x); }

double eval_tan(double x, UNUSED double u) { return tan(x); }

double eval_asin(double x, UNUSED double u) { return asin(x); }

double eval_acos(double x, UNUSED double u) { return acos(x); }

double eval_atan(double x, UNUSED double u) { return atan(x); }

double eval_sqrt(double x, UNUSED double u) { return sqrt(x); }

double eval_log(double x, UNUSED double u) { return log(x); }

double eval_ln(double x, UNUSED double u) { return log10(x); }
