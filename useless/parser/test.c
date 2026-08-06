#include "headers/useless_util.h"
#include <stdio.h>
int main() {
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
