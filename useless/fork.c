#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
int main(){
  printf("lol %d", 33);
  int rc = fork();
  if (rc < 0){
    printf("error (%d)",(int) getpid());
  } else if (rc == 0){
    printf("success %d ", (int) getpid());
  } else {
    printf("parent %d", (int) getpid());
  }
  return 0; 
}
