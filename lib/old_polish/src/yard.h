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

void push_opstack(struct operator_type *op, char *error_message);

void push_numstack(double num, char *error_message);

double pop_numstack(char *error_message);

void shunt_op(struct operator_type *op, char *error_message);

int isdigit_or_decimal(int c);

double yard(char *s, char *error_message);

struct operator_type *pop_opstack(char *error_message);

#endif  // SRC_YARD_H_
