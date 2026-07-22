#include <stdlib.h>
#include <stdio.h>
int main(){
  void * ptr = malloc(100 * 4);
  int * arr;
  arr = (int *)ptr;
  for (int i = 0; i < 100; i++){
    arr[i] = i;
  }
  for (int i = 0; i < 100; i++){
    printf("%d, ", arr[i]);
  }
  free(ptr);
  return 0;
}
