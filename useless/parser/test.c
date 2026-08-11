#include "headers/expression.h"
#include "headers/lexer.h"
#include "headers/useless_util.h"
#include <stdio.h>
int main() {
  Number number = {INTEGER, {.integer = 33}};
  printf("%f\n", number.numeric.decimal);
  printf("%f\n", (float)number.numeric.integer);
  if (1)
    return 0;
  char c = ' ';
  char hex = '\x32';
  if (c == hex) {
    printf("gleich");
    return 0;
  }
  String s = toHex(hex);
  printf("_%c:%d\n", hex, hex);
  printf("%s\n", s.string);
  return 0;
}
