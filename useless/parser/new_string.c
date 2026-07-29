#include "headers/new_string.h"
#include "headers/useless_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
int main() {
  char a = '3';
  char *ptr = &a;
  printf("length: %ld\n", strlen(ptr));
  printf("length: %ld\n", strlen(&a));
  if (1)
    return 0;
  String s0 = createStr("");
  String s1 = createStr("");
  printNull(&s0);
  printNull(&s1);
  for (int i = 0; i < 10; i++) {
    char str[] = {i + '0'};
    String otherStr = createStr(str);

    appendStr(&s1, &otherStr);
  }
  printNull(&s0);
  printNull(&s1);
  freeStr(&s0);
  freeStr(&s1);
  return 0;
}
*/
String createStr(char *str) {
  String s;
  s.size = strlen(str) + 1;
  s.capacity = roundCapacity(s.size);
  char *strDest = malloc(s.capacity);
  s.str = strDest;
  memcpy(s.str, str, s.size);
  return s;
}
void appendCStr(String *str, char *content) {
  if (!*content) {
    return;
  }
  printf("\nappend debug: ");
  printdebug(content);
  printf("\nend debug");
  int size = strlen(content) + 1;
  if (str->size + size > str->capacity) {
    int capacity = roundCapacity(str->size + size - 1);
    char *newStr = (char *)realloc(str->str, capacity);
    if (newStr == NULL) {
      return;
    }
    str->str = newStr;
    str->capacity = capacity;
  }
  memcpy(str->str + str->size - 1, content, size);
  str->size += size - 1;
}
void appendChar(String *str, char c) {
  if (!c) {
    return;
  }
  if (str->size + 1 > str->capacity) {
    int capacity = roundCapacity(str->size + 1);
    char *newStr = (char *)realloc(str->str, capacity);
    if (newStr == NULL) {
      return;
    }
    str->str = newStr;
    str->capacity = capacity;
  }
  memcpy(str->str + str->size - 1, &c, 1);
  str->size++;
}
void appendStr(String *str, String *otherStr) {
  if (!otherStr->str)
    return;
  if (str->size + otherStr->size > str->capacity) {
    int capacity = roundCapacity(str->size + otherStr->size - 1);
    char *newStr = (char *)realloc(str->str, capacity);
    if (newStr == NULL)
      return;
    str->str = newStr;
    str->capacity = capacity;
  }
  memcpy(str->str + str->size - 1, otherStr->str, otherStr->size);
  str->size += otherStr->size - 1;
}
int toInt(String *str) {
  int value = 0;
  for (int i = 0; i < str->size - 1; i++) {
    value *= 10;
    value += (str->str[i] - '0');
  }
  return value;
}
void printNull(String *str) {
  for (int i = 0; i < str->size; i++) {
    if (str->str[i]) {
      printf("%c", str->str[i]);
      continue;
    }
    printf("$");
  }
}
void printStr(String *str) {
  printf("string():\n->str: ");
  printNull(str);
  printf("\n->size: %d\n->capacity: %d\n", str->size, str->capacity);
}
void printHidden(char *str) {
  while (*str) {
    if ((*str < ' ') || (*str == 127)) {
      printf(" hidden: %d,", *str);
      str++;
      continue;
    }
    printf("%c", *str);
    str++;
  }
}
void setStrEmpty(String *str) {
  printf("setEmpty called\n");
  str->str[0] = '\0';
  str->size = 1;
}
int isEmpty(String *str) { return str->size == 1; }
int roundCapacity(int s) { return s + (10 - s % 10); }
void freeStr(String *str) { free(str->str); }
