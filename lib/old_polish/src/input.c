//
// Created by Gladis Ariane on 4/3/22.
//

#include "input.h"
#include <stdio.h>

int contains_x(char *s) {
  if (strchr(s, 'x')) {
    return 1;
  }
  return 0;
}

char *truncate_trig(char *z) {
  char *s = malloc(strlen(z) * sizeof(char));
  s = strcpy(s, z);
  for (unsigned long i = 0; i < strlen(s); ++i) {
    if (s[i] == 's' && s[i + 1] == 'i') {  // sin
      memmove(&s[i + 1], &s[i + 3], strlen(s) - i);
    }
    if (s[i] == 'c' && s[i + 1] == 'o') {  // cos
      memmove(&s[i + 1], &s[i + 3], strlen(s) - i);
    }
    if (s[i] == 't' && s[i + 1] == 'a') {  // tan
      memmove(&s[i + 1], &s[i + 3], strlen(s) - i);
    }
    if (s[i] == 'a' && s[i + 1] == 's' && s[i + 2] == 'i') {  // asin
      memmove(&s[i + 1], &s[i + 4], strlen(s) - i);
      s[i] = 'b';
    }
    if (s[i] == 'a' && s[i + 1] == 'c' && s[i + 2] == 'o') {  // acos
      memmove(&s[i + 1], &s[i + 4], strlen(s) - i);
      s[i] = 'd';
    }
    if (s[i] == 'a' && s[i + 1] == 't' && s[i + 2] == 'a') {  // atan
      memmove(&s[i + 1], &s[i + 4], strlen(s) - i);
      s[i] = 'e';
    }
    if (s[i] == 's' && s[i + 1] == 'q' && s[i + 2] == 'r') {  // sqrt
      memmove(&s[i + 1], &s[i + 4], strlen(s) - i);
      s[i] = 'q';
    }
    if (s[i] == 'l' && s[i + 1] == 'n') {  // ln
      memmove(&s[i + 1], &s[i + 2], strlen(s) - i);
      s[i] = 'n';
    }
    if (s[i] == 'l' && s[i + 1] == 'o' && s[i + 2] == 'g') {  // log
      memmove(&s[i + 1], &s[i + 3], strlen(s) - i);
      s[i] = 'l';
    }
  }
  return s;
}

char *str_replace(char *orig, char *rep, char *with) {
//    fmt = "1+(6-5) * x";
//    fmt = "1+(6-5) * %lf";
  char *result;  // the return string
  char *ins;     // the next insert point
  char *tmp;     // varies
  size_t len_rep;   // length of rep (the string to remove)
  size_t len_with;  // length of with (the string to replace rep with)

  int count;    // number of replacements

  // sanity checks and initialization
  if (!orig || !rep)
    return NULL;
  len_rep = strlen(rep);
  if (len_rep == 0)
    return NULL;  // empty rep causes infinite loop during count
  if (!with)
    with = "";
  len_with = strlen(with);

  // count the number of replacements needed
  ins = orig;
  for (count = 0; (tmp = strstr(ins, rep)); ++count) {
    ins = tmp + len_rep;
  }
  tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);
  if (!result)
    return NULL;

  //  first time through the loop, all the variable are set correctly
  //  from here on,
  //    tmp points to the end of the result string
  //    ins points to the next occurrence of rep in orig
  //    orig points to the remainder of orig after "end of rep"
  while (count--) {
    size_t len_front;  // distance between rep and end of last rep
    ins = strstr(orig, rep);
    len_front = ins - orig;
    tmp = strncpy(tmp, orig, len_front) + len_front;
    tmp = strcpy(tmp, with) + len_with;
    orig += len_front + len_rep;  // move to next "end of rep"
  }
  strcpy(tmp, orig);
  return result;
}

char *format_x(char *fmt, double val) {
//    fmt = "1+(6-5) * %lf";
//    fmt = "1+(6-5) * val";
  char *res;
  asprintf(&res, fmt, val);
  return res;
}
