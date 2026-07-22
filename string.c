#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct string {
  char *str;
  int size;
};
struct string createStr(char *str);
void appendCStr(struct string *str, char *content);
void appendStr(struct string *str, struct string *otherStr);
void freeStr(struct string *str);
int main() {
  struct string s0 = createStr("cstring: ");
  struct string s1 = createStr("String: ");
  printf("%s\n", s0.str);
  printf("%s\n", s1.str);
  for (int i = 0; i < 10; i++) {
    char str[] = {i + '0'};
    struct string otherStr = createStr(str);
    appendCStr(&s0, str);
    appendStr(&s1, &otherStr);
  }
  printf("%s\n", s0.str);
  printf("%s\n", s1.str);
  freeStr(&s0);
  freeStr(&s1);
  return 0;
}
struct string createStr(char *str) {
  struct string s;
  s.size = strlen(str);
  char *strDest = malloc(s.size + 1);
  strcpy(strDest, str);
  s.str = strDest;
  return s;
}
void appendCStr(struct string *str, char *content) {
  int size = strlen(content);
  if (!size)
    return;
  char *newStr = (char *)realloc(str->str, str->size + size + 1);
  if (newStr == NULL)
    return;
  str->str = newStr;
  strcat(str->str, content);
}
void appendStr(struct string *str, struct string *otherStr) {
  if (!otherStr->size)
    return;
  char *newStr = (char *)realloc(str->str, str->size + otherStr->size + 1);
  if (newStr == NULL)
    return;
  str->str = newStr;
  strcat(str->str, otherStr->str);
}
void freeStr(struct string *str) { free(str->str); }
