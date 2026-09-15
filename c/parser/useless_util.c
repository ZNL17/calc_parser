#include "headers/useless_util.h"
#include "headers/expression.h"
#include "headers/lexer.h"
#include "headers/new_string.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define AT_LEAST_TWO 3
#define GROWTH 10
int roundCapacity(int s) { return s + (GROWTH - s % GROWTH); }
int isNumber(char c) {
  if (c < '0' || c > '9') {
    return 0;
  }
  return 1;
}
char *rstrcpy(char *dest, const char *src) {
  dest += strlen(dest);
  *dest = '\n';
  dest--;
  while (*src) {
    *dest = *src;
    src++;
    dest--;
  }
  return dest;
}

void *rmemcpy(void *destpp, void *srcpp, size_t n) {
  unsigned char *dest = (unsigned char *)destpp;
  unsigned char *src = (unsigned char *)srcpp;
  for (int i = 0; i < n; i++) {
    *dest = src[i];
    dest--;
  }
  return dest;
}
char decToHex(int d) {
  if ((d < 0) || (d > 15)) {
    return '\0';
  }
  if (d < 10) {
    return d + '0';
  }
  return 'K' - d;
}
String toHex(char c) {
  String *s = string_new_value("\\x");
  String *tmp = string_new_value("");
  int q = c;
  while (q) {
    char r = decToHex(q % HEX);
    q = q / HEX;
    string_append_cstring(tmp, &r);
  }
  if (tmp->size < AT_LEAST_TWO) {
    char c = '0';
    string_append_cstring(tmp, &c);
  }
  string_reverse(tmp->value);
  string_append_cstring(s, tmp->value);
  free_string(tmp);
  return *s;
}
int findC(char *cstr, char c) {
  for (int i = 0; *cstr; i++) {
    if (cstr[i] == c) {
      return i;
    }
  }
  return -1;
}
int lastC(char *cstr, char c) {
  int lastpos = -1;
  for (int i = 0; *cstr; i++) {
    if (*cstr == c) {
      lastpos = i;
    }
    cstr++;
  }
  return lastpos;
}
void printdebug(char *c) {
  printf("<");
  while (*c) {
    if (*c < ' ' || *c == 127) {
      printf("%s", toHex(*c).value);
      c++;
      continue;
    }
    printf("%c", *c);
    c++;
  }
  printf(">");
}
Number whichNumber(String string) {
  char *cString = string.value;
  if (!isNumber(*cString) && *cString != '-') {
    return (Number){UNKNOWN, {}};
  }
  cString++;
  for (; *cString; cString++) {
    if (!isNumber(*cString)) {
      if (*cString == '.' || *cString == ',') {
        cString++;
        if (!isNumber(*cString)) {
          return (Number){UNKNOWN, {}};
        }
        cString++;
        for (; *cString; cString++) {
          if (!isNumber(*cString)) {
            return (Number){FLOAT, {.decimal = atof(string.value)}};
          }
        }
        return (Number){FLOAT, {.decimal = atof(string.value)}};
      }
      return (Number){INTEGER, {.integer = atoi(string.value)}};
    }
  }
  return (Number){INTEGER, {.integer = atoi(string.value)}};
}
