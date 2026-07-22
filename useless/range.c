#include <stdio.h>
void repeat(char c, int count);
void repeatSeq(char *charSeq, int count);
int main() {
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
