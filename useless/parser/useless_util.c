#include "headers/useless_util.h"
#include "headers/new_string.h"
#include <stddef.h>
#include <stdio.h>
#define AT_LEAST_TWO 3

int isNumber(char c) {
  if (c < '0' || c > '9') {
    return 0;
  }
  return 1;
}
void reverseCStr(char *cstr) {
  int size = strlen(cstr);
  if (size < 2)
    return;
  char ctmp;
  for (int i = 0; i < size / 2; i++) {
    ctmp = cstr[size - 1 - i];
    cstr[size - 1 - i] = cstr[i];
    cstr[i] = ctmp;
  }
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
  String s = createStr("\\x");
  String tmp = createStr("");
  int q = c;
  while (q) {
    char r = decToHex(q % HEX);
    q = q / HEX;
    appendCStr(&tmp, &r);
  }
  if (tmp.size < AT_LEAST_TWO) {
    char c = '0';
    appendCStr(&tmp, &c);
  }
  reverseCStr(tmp.str);
  appendStr(&s, &tmp);
  freeStr(&tmp);
  return s;
}
void printdebug(char *c) {
  printf("<");
  while (*c) {
    if (*c < ' ' || *c == 127) {
      printf("%s", toHex(*c).str);
      c++;
      continue;
    }
    printf("%c", *c);
    c++;
  }
  printf(">");
}
