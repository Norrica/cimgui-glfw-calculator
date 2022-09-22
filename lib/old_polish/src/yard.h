#ifndef SRC_YARD_H_
#define SRC_YARD_H_

#include "operations.h"

#define CHECKMALLOC(var) if ((var) == NULL) {printf("ERROR: malloc\n"); abort();}
#define MAXOPSTACK  1024
#define MAXNUMSTACK 1024
enum {
  NONE = 0, LEFT, RIGHT
};

struct operator_type {
  char op;
  int prec;
  int assoc;
  int unary;

  double (*eval)(double a1, double a2);
};

void push_opstack(struct operator_type *op);

void push_numstack(double num);

double pop_numstack();

void shunt_op(struct operator_type *op);

int isdigit_or_decimal(int c);

double yard(char *s, int *is_error);

struct operator_type *pop_opstack();

#endif  // SRC_YARD_H_
