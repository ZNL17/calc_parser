#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
void showDir(char *targetDir);
void allDir(char *targetDir, int LEVEL);
int delete();
int create();
int move();
void printLevel(char *name, int LEVEL);
int example();
int main(int argc, char **argv) {
  allDir("/home/wisdomkingraphael/Projects/", 0);
  return 0;
}

void showDir(char *targetDir) {
  DIR *FD;
  struct dirent *in_file;
  FD = opendir(targetDir);
  if (FD == NULL) {
    printf("error lol");
    return;
  }
  while ((in_file = readdir(FD))) {
    printf("%s\n", in_file->d_name);
  }
  closedir(FD);
  return;
}
void allDir(char *targetDir, int LEVEL) {
  printLevel(targetDir, LEVEL++);
  DIR *FD;
  struct dirent *in_file;
  FD = opendir(targetDir);
  if (FD == NULL)
    return;
  while ((in_file = readdir(FD))) {
    printf("<%s>\n", in_file->d_name);
    char buffer[512];
    strcpy(buffer, targetDir);
    strcat(buffer, "/");
    strcat(buffer, in_file->d_name);
    switch (in_file->d_type) {
    case DT_DIR:
      if (strcmp(in_file->d_name, ".") || strcmp(in_file->d_name, "..")) {
        printf("cmp . || ..\n");
        continue;
      }
      printf("alldir\n");
      allDir(buffer, LEVEL);
      break;
    case DT_REG:
      printf("file\n");
      printLevel(buffer, LEVEL);
      break;
    }
    printf("continue\n");
  }
  closedir(FD);
  return;
}
void printLevel(char *name, int LEVEL) {
  char padding[LEVEL + 1];
  for (int i = 0; i < LEVEL; i++) {
    padding[i] = ' ';
  }
  padding[LEVEL] = '\0';
  printf("%s -> %s\n", padding, name);
}
int example() {
  DIR *FD;
  struct dirent *in_file;
  char *target_dir = ".";
  FD = opendir(target_dir);
  if (FD == NULL) {
    fprintf(stderr, "Error: Failed to open input directory - %s \n",
            strerror(errno));
    return 1;
  }
  while ((in_file = readdir(FD))) {
    struct stat buffer;
    int status;
    status = stat(in_file->d_name, &buffer);
    if (status == -1) {
      fprintf(stderr, "Error: failed to stat item - %s\n", strerror(errno));
      return 1;
    }
    printf("%s is file \n", in_file->d_name);
    if (buffer.st_mode & S_IFREG) {
    }
  }
  closedir(FD);
  return 0;
}
