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
String *string_add_value_new(char *cstring) {
  String *s = malloc(sizeof(String));
  s->size = strlen(cstring) + 1;
  s->capacity = roundCapacity(s->size);
  char *strDest = malloc(s->capacity);
  s->string = strDest;
  memcpy(s->string, cstring, s->size);
  return s;
}
String *string_new() {
  String null = {NULL, 0, 0};
  String *string = malloc(sizeof(String));
  if (string == NULL) {
    return (String *)NULL;
  }
  int capacity = roundCapacity(0);
  char *ptr = (char *)malloc(sizeof(String));
  if (ptr == NULL) {
    return (String *)NULL;
  }
  *ptr = '\0';
  null.string = ptr;
  null.size++;
  null.capacity = capacity;
  memcpy(ptr, &null, sizeof(String));
  return string;
}
void string_set_cstrings(String *string, char *cstring) {
  if (!cstring) {
    return;
  }
  int size = strlen(cstring) + 1;
  if (size > string->capacity) {
    int capacity = roundCapacity(size);
    char *string_ptr = (char *)realloc(string->string, capacity);
    if (string_ptr == NULL) {
      return;
    }
    string->string = string_ptr;
    string->capacity = capacity;
  }
  strcpy(string->string, cstring);
  string->size = size;
}
String *substring(char *cstring, int start, int end) {
  int size = strlen(cstring) + 1;
  int length = (end - start);
  if (length < 1 || length > size) {
    return (String *)NULL;
  }
  if (start + (end - start) > size) {
    return (String *)NULL;
  }
  String *string = string_new();
  if (length + 1 > string->capacity) {
    int capacity = roundCapacity(length + 1);
    char *ptr = (char *)realloc(string->string, capacity);
    if (ptr == NULL) {
      return (String *)NULL;
    }
    string->string = ptr;
    string->capacity = capacity;
  }
  memcpy(string->string, cstring + start, length);
  string->string[start + length] = '\0';
  string->size = length + 1;
  return string;
}
void strip_mark(String *string) {
  if (!string->string) {
    return;
  }
  int start = sequence(string->string, '"', 0);
  if (start == -2) {
    string_empty(string);
    return;
  }
  int end = sequence(string->string, '"', 1);
  if (end == -2) {
    string_empty(string);
    return;
  }

  printf("amem\n");
  if (start == -1 && end == string->size) {
    return;
  }
  int size = (end - 2) - (start + 1);
  memcpy(string->string, string->string + (start + 1), size);
  string->string[(start + 1) + size] = '\0';
  string->size = size + 1;
}
int sequence(char *cstring, char c, int reverse) {
  printf("los\n");
  int size = strlen(cstring);
  int offset = (size)*reverse;
  int iter = 1 + (reverse * -2);
  int j = 0;
  for (int i = 0; i < size; i++) {
    if (cstring[offset + j] != c) {
      return offset + j + (-1 + (reverse * 2));
    }
    j += iter;
  }
  return -2;
}
void appendCStr(String *str, char *content) {
  if (!*content) {
    return;
  }
  int size = strlen(content) + 1;
  if (str->size + size > str->capacity) {
    int capacity = roundCapacity(str->size + size - 1);
    char *newStr = (char *)realloc(str->string, capacity);
    if (newStr == NULL) {
      return;
    }
    str->string = newStr;
    str->capacity = capacity;
  }
  memcpy(str->string + str->size - 1, content, size);
  str->size += size - 1;
}
void appendChar(String *str, char c) {
  if (!c) {
    return;
  }
  if (str->size + 2 > str->capacity) {
    int capacity = roundCapacity(str->size + 2 - 1);
    char *newStr = (char *)realloc(str->string, capacity);
    if (newStr == NULL) {
      return;
    }
    str->string = newStr;
    str->capacity = capacity;
  }
  memcpy(str->string + str->size - 1, &c, 1);
  str->string[str->size + 1] = '\0';
  str->size++;
}
void appendStr(String *str, String *otherStr) {
  if (!otherStr->string)
    return;
  if (str->size + otherStr->size > str->capacity) {
    int capacity = roundCapacity(str->size + otherStr->size - 1);
    char *newStr = (char *)realloc(str->string, capacity);
    if (newStr == NULL)
      return;
    str->string = newStr;
    str->capacity = capacity;
  }
  memcpy(str->string + str->size - 1, otherStr->string, otherStr->size);
  str->size += otherStr->size - 1;
}
int toInt(String *str) {
  int value = 0;
  for (int i = 0; i < str->size - 1; i++) {
    value *= 10;
    value += (str->string[i] - '0');
  }
  return value;
}
StringList *stringList_new() {
  StringList *ptr = (StringList *)malloc(sizeof(StringList));
  if (ptr == NULL) {
    return (StringList *)NULL;
  }
  String *listptr = (String *)malloc(sizeof(String) * 10);
  if (listptr == NULL) {
    return (StringList *)NULL;
  }
  ptr->strings = (String **)listptr;
  ptr->size = 0;
  ptr->capacity = 10;
  return ptr;
}
void stringList_append(StringList *stringList, String *string) {
  if (!string->string) {
    return;
  }
  if (stringList->size + 1 > stringList->capacity) {
    int capacity = roundCapacity(stringList->size + 1);
    StringList *ptr = (StringList *)realloc(stringList->strings - 1, capacity);
    if (ptr == NULL) {
      return;
    }
    stringList->capacity = capacity;
  }
  stringList->strings[stringList->size++] = string;
}
void stringList_pop(StringList *stringList) {
  if (!stringList->size) {
    return;
  }

  free_string(stringList->strings[stringList->size]);
  stringList->strings[--stringList->size] = NULL;
}
StringList *split(char *cstring, char c, int count) {
  if (!*cstring) {
    return (StringList *)NULL;
  }
  StringList *stringList = stringList_new();
  int i = 0;
  int j = 0;
  for (; count && cstring[j]; j++) {
    if (cstring[j] == c) {
      String *cell = substring(cstring, i, j);
      stringList_append(stringList, cell);
      i = j + 1;
      count--;
    }
  }
  if (count < 0 && !cstring[j]) {
    String *cell = substring(cstring, i, j), cstring, c, count;
    stringList_append(stringList, cell);
  }

  return stringList;
}
void freeStringList(StringList *stringList) { free(--stringList->strings); }
void printNull(String *str) {
  for (int i = 0; i < str->size; i++) {
    if (str->string[i]) {
      printf("%c", str->string[i]);
      continue;
    }
    printf("$");
  }
}
void printStr(String *str) {
  printf("string():\n->string: ");
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
void string_empty(String *str) {
  str->string[0] = '\0';
  str->size = 1;
}
int isEmpty(String *str) { return str->size == 1; }
void free_string(String *string) {
  free(string->string);
  free(string);
}
