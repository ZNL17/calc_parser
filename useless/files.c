#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void readFile(char * fileName);
int main(){
  readFile("files.c");
  return 0;
}

void readFile(char * fileName){
  FILE * fptr;
  int BUFFER= 64;
  char data[BUFFER];
  fptr = fopen(fileName, "r");
  if (fptr == NULL){
    printf("The file %s counld't be opened", fileName);
    return;
  }
  while (fgets(data, BUFFER, fptr)){
    printf("%s", data);
  }
  fclose(fptr);
  return;
}
