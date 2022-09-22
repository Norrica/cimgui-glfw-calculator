
#include <stdlib.h>
#include "input.h"
#include "yard.h"

int build_graph(int x_left, int x_right, char *s, double *result_arr,char*error_message) {
  char *ss = truncate_trig(s);  //  contains 'x'
  char *sss = str_replace(ss, "x", "%lf");
  int is_error = 0;
  for (int x_count = x_left, i = 0; x_count < x_right; x_count += 1, i++) {
    char *str = format_x(sss, x_count);
    result_arr[i] = yard(str,error_message);
    free(str);
  }
  free(sss);
  free(ss);
  //free(s);
  return is_error;
}

double calc_res(char *s, char *is_error) {
  char *ss = truncate_trig(s);
  double res = yard(ss, is_error);
  free(ss);
  return res;

}