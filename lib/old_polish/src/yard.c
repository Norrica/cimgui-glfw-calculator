//
// Created by Gladis Ariane on 4/2/22.
//



#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "yard.h"
#include "operations.h"

struct operator_type operators[] = {

    {'+', 5, LEFT, 0, eval_add}, {'-', 5, LEFT, 0, eval_sub},
    {'*', 7, LEFT, 0, eval_mul}, {'/', 7, LEFT, 0, eval_div},
    {'^', 9, LEFT, 0, eval_pow}, {'%', 7, LEFT, 0, eval_mod},
    {'~', 10, RIGHT, 1, eval_neg}, {'s', 10, RIGHT, 1, eval_sin},
    {'c', 10, RIGHT, 1, eval_cos}, {'t', 10, RIGHT, 1, eval_tan},
    {'b', 10, RIGHT, 1, eval_asin}, {'d', 10, RIGHT, 1, eval_acos},
    {'e', 10, RIGHT, 1, eval_atan}, {'n', 10, RIGHT, 1, eval_ln},
    {'l', 10, RIGHT, 1, eval_log}, {'q', 10, RIGHT, 1, eval_sqrt},
    {'(', 0, NONE, 0, NULL}, {')', 0, NONE, 0, NULL},
    //        {'x', 0,  NONE,  0, NULL},

//        {'x', 0,  NONE,  0, NULL},
};
struct operator_type *opstack[MAXOPSTACK];
int oper_stack_size = 0;
double numstack[MAXNUMSTACK];
int num_stack_size = 0;

struct operator_type *getop(char ch) {
  for (unsigned long i = 0; i < sizeof operators / sizeof operators[0]; ++i) {
    if (operators[i].op == ch) return operators + i;
  }
  return NULL;
}

void push_opstack(struct operator_type *op, char *error_message) {
  if (oper_stack_size > MAXOPSTACK - 1) {
    strcpy(error_message, "ERROR: Operator stack overflow");

  }
  opstack[oper_stack_size++] = op;
}

struct operator_type *pop_opstack(char *error_message) {
  if (!oper_stack_size) {
    strcpy(error_message, "ERROR: Operator stack empty");

  }
  return opstack[--oper_stack_size];
}

void push_numstack(double num, char *error_message) {
  if (num_stack_size > MAXNUMSTACK - 1) {
    strcpy(error_message, "ERROR: Number stack overflow");

  }
  numstack[num_stack_size++] = num;
}

double pop_numstack(char *error_message) {
  if (!num_stack_size) {
    strcpy(error_message, "ERROR: Number stack empty");
    return NAN;
  }
  return numstack[--num_stack_size];
}

void shunt_op(struct operator_type *op, char *error_message) {
  struct operator_type *pop;
  double n1, n2;
  if (op->op == '(') {
    push_opstack(op, error_message);
    return;
  } else if (op->op == ')') {
    while (oper_stack_size > 0 && opstack[oper_stack_size - 1]->op != '(') {
      pop = pop_opstack(error_message);
      n1 = pop_numstack(error_message);
      if (pop->unary) {
        push_numstack(pop->eval(n1, 0), error_message);
      } else {
        n2 = pop_numstack(error_message);
        push_numstack(pop->eval(n2, n1), error_message);
      }
    }
    if (!(pop = pop_opstack(error_message)) || pop->op != '(') {
      strcpy(error_message, "ERROR: Stack error. No matching \'(\'");

    }
    return;
  }
  if (op->assoc == RIGHT) {
    while (oper_stack_size && op->prec < opstack[oper_stack_size - 1]->prec) {
      pop = pop_opstack(error_message);
      n1 = pop_numstack(error_message);
      if (pop->unary) {
        push_numstack(pop->eval(n1, 0), error_message);
      } else {
        n2 = pop_numstack(error_message);
        push_numstack(pop->eval(n2, n1), error_message);
      }
    }
  } else if (op->assoc == LEFT) {
    while (oper_stack_size && op->prec < opstack[oper_stack_size - 1]->prec) {
      pop = pop_opstack(error_message);
      n1 = pop_numstack(error_message);
      if (pop->unary) {
        push_numstack(pop->eval(n1, 0), error_message);
      } else {
        n2 = pop_numstack(error_message);
        push_numstack(pop->eval(n2, n1), error_message);
      }
    }
  } else {
    while (oper_stack_size && op->prec <= opstack[oper_stack_size - 1]->prec) {
      pop = pop_opstack(error_message);
      n1 = pop_numstack(error_message);
      if (pop->op == 'x') {
        push_opstack(pop, error_message);
      }
      if (pop->unary) {
        push_numstack(pop->eval(n1, 0), error_message);
      } else {
        n2 = pop_numstack(error_message);
        push_numstack(pop->eval(n2, n1), error_message);
      }
    }
  }
  push_opstack(op, error_message);
}

int isdigit_or_decimal(int c) {
  if (c == '.' || isdigit(c))
    return 1;
  else
    return 0;
}

float yard(char *s, char *error_message) {
  num_stack_size = 0;
  oper_stack_size = 0;
  char *expr;
  char *tstart = NULL;
  struct operator_type
      startop = {'X', 0, NONE, 0, NULL};  /* Dummy operator to mark start */
  struct operator_type *op = NULL;
  struct operator_type *lastop = &startop;
  for (expr = s; *expr; ++expr) {
    if (!tstart) {
      if ((op = getop(*expr))) {
        if (lastop && (lastop == &startop || lastop->op != ')')) {
          switch (op->op) {
            case '-':op = getop('~');
              break;
            case 's':op = getop('s');
              break;
            case 'c':op = getop('c');
              break;
            case 't':op = getop('t');
              break;
            case 'r':op = getop('r');
              break;
            case 'g':op = getop('g');
              break;
            case 'x':op = getop('x');
              break;
            case '+':
              op = getop('+');
              break;

            case '*':
              op = getop('*');
              break;
            case '/':
              op = getop('/');
              break;
            case '^':
              op = getop('^');
              break;
            case '%':
              op = getop('%');
              break;
            case '~':
              op = getop('~');
              break;
            case 'b':
              op = getop('b');
              break;
            case 'd':
              op = getop('d');
              break;
            case 'e':
              op = getop('e');
              break;
            case 'n':
              op = getop('n');
              break;
            case 'l':
              op = getop('l');
              break;
            case 'q':
              op = getop('q');
              break;
            //case '(':
            //  op = getop('(');
            //  break;
            //case ')':
            //  op = getop(')');
            //  break;

            default:break;
          }
        }
        shunt_op(op, error_message);
        lastop = op;
      } else if (isdigit_or_decimal(*expr)) {
        tstart = expr;
      } else if (!isspace(*expr)) {
        strcpy(error_message, "ERROR: Syntax error");

        return NAN;
      }
    } else {
      if (isspace(*expr)) {
        push_numstack(atof(tstart), error_message);
        tstart = NULL;
        lastop = NULL;
      } else if ((op = getop(*expr))) {
        push_numstack(atof(tstart), error_message);
        tstart = NULL;
        shunt_op(op, error_message);
        lastop = op;
      } else if (!isdigit_or_decimal(*expr)) {
        strcpy(error_message, "ERROR: Syntax error");
        return NAN;
      }
    }
  }
  if (tstart)
    push_numstack(atof(tstart), error_message);
  while (oper_stack_size) {
    double n1;
    op = pop_opstack(error_message);
    n1 = pop_numstack(error_message);
    if (op->unary) {
      push_numstack(op->eval(n1, 0), error_message);
    } else {
      double n2 = pop_numstack(error_message);
      push_numstack(op->eval(n2, n1), error_message);
    }
  }
  if (num_stack_size != 1) {
    fprintf(stderr,
            "ERROR: Number stack has %d elements after evaluation. Should be 1.",
            num_stack_size);

    return NAN;
  }

  return (float)numstack[0];

}
