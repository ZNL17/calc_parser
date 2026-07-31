#include <stdio.h>
void repeat(char c, int count);
void repeatSeq(char *charSeq, int count);
int main() {
  char *c = "1*3-";
  while (*c) {
    printf("'0' is %d\n", '0');
    printf("'9' is %d\n", '9');
    printf("'%c' is %d\n", *c, *c);

    if ((*c < '0') || (*c > '9')) {
      printf("not num\n");
      c++;
      continue;
    }
    printf("number\n");
    c++;
  }
  if (1) {
    return 0;
  }
  int counter = 10;
  for (int i = 0; i < counter; i++) {
    printf("%d: ", 1 << i);
    repeatSeq("x", i << i);
    printf("\n");
  }
  return 0;
}
void repeat(char c, int count) {
  for (int i = count; i > 0; i--) {
    printf("%c", c);
  }
}
void repeatSeq(char *charSeq, int count) {
  for (int i = count; i > 0; i--) {
    printf("%s", charSeq);
  }
}
