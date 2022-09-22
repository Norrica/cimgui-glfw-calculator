
#include <stdlib.h>
#include "input.h"
#include "yard.h"

float * build_graph(int x_left, int x_right, char *s, char *error_message) {

  float *result_arr = malloc((x_right - x_left) * sizeof(float));
  char *ss = truncate_trig(s);
  char *sss = str_replace(ss, "x", "%lf");
  for (int x_count = x_left, i = 0; x_count < x_right; x_count += 1, i++) {
    char *str = format_x(sss, x_count);
    result_arr[i] = yard(str, error_message);
    free(str);
  }
  free(sss);
  free(ss);
  //free(s);
  return result_arr;
}

double calc_res(char *s, char *is_error) {
  char *ss = truncate_trig(s);
  double res = yard(ss, is_error);
  free(ss);
  return res;

}