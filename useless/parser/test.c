#include "headers/new_string.h"
#include "headers/useless_util.h"
#include <stdio.h>
void printHex(char *in);
int main() {
  char *string = "ab\ndf\x03, name\x02";
  printHex(string);
  return 0;
}
void printHex(char *in) {
  while (*in) {
    if (*in < ' ' || *in == 127) {
      String s = toHex(*in);
      printf("%s", s.str);
      in++;
      continue;
    }
    printf("%c", *in);
    in++;
  }
}
