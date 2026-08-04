#include <stdio.h>
int main() {
  int i = 0;
  int *ptr = &i;
  for (int j = 0; j < 10; j++) {
    (*ptr)++;
  }
  printf("result: %d", *ptr);
  return 0;
}
