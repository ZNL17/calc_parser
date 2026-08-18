#include "headers/new_string.h"
#include "headers/useless_util.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
String *string_new() {
  String *ptr = malloc(sizeof(String));
  if (ptr == NULL) {
    return (String *)NULL;
  }
  int capacity = 10;
  char *cptr = (char *)malloc(capacity);
  if (ptr == NULL) {
    return (String *)NULL;
  }
  *cptr = '\0';
  *ptr = (String){cptr, 1, capacity};
  return ptr;
}
String *string_new_value(char *cstring) {
  String *s = malloc(sizeof(String));
  s->size = strlen(cstring) + 1;
  s->capacity = roundCapacity(s->size);
  char *strDest = malloc(s->capacity);
  s->value = strDest;
  memcpy(s->value, cstring, s->size);
  return s;
}
void string_set_cstrings(String *string, char *cstring) {
  if (!cstring) {
    return;
  }
  int size = strlen(cstring) + 1;
  if (size > string->capacity) {
    int capacity = roundCapacity(size);
    char *string_ptr = (char *)realloc(string->value, capacity);
    if (string_ptr == NULL) {
      return;
    }
    string->value = string_ptr;
    string->capacity = capacity;
  }
  strcpy(string->value, cstring);
  string->size = size;
}

void string_append_cstring(String *str, char *content) {
  if (!*content) {
    return;
  }
  int size = strlen(content) + 1;
  if (str->size + size > str->capacity) {
    int capacity = roundCapacity(str->size + size - 1);
    char *newStr = (char *)realloc(str->value, capacity);
    if (newStr == NULL) {
      return;
    }
    str->value = newStr;
    str->capacity = capacity;
  }
  memcpy(str->value + str->size - 1, content, size);
  str->size += size - 1;
}
void string_append_char(String *str, char c) {
  if (!c) {
    return;
  }
  if (str->size + 1 > str->capacity) {
    int capacity = roundCapacity(str->size + 1);
    char *newStr = (char *)realloc(str->value, capacity);
    if (newStr == NULL) {
      return;
    }
    str->value = newStr;
    str->capacity = capacity;
  }
  memcpy(str->value + str->size - 1, &c, 1);
  str->value[str->size] = '\0';
  str->size++;
}
void string_append_fmt_string(String *string, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  va_list args_cpy;
  va_copy(args_cpy, args);

  int len = vsnprintf(NULL, 0, fmt, args_cpy);
  va_end(args_cpy);
  if (len < 0) {
    va_end(args);
    return;
  }
  printf("len: %d", len);
  if (string->size + len > string->capacity) {
    int capacity = roundCapacity(string->size + len);
    char *ptr = (char *)realloc(string->value, capacity);
    if (ptr == NULL) {
      return;
    }
    string->value = (char *)ptr;
    string->capacity = capacity;
  }
  vsnprintf(string->value + string->size - 1, len + 1, fmt, args);
  string->size = strlen(string->value) + 1;
  va_end(args);
}
void string_append_fmt_cstring(char *cstring, size_t maxlen, const char *fmt,
                               ...) {
  va_list args;
  va_start(args, fmt);
  int len = snprintf(NULL, 0, fmt, args);
  if (len < 0) {
    return;
  }
  int clen = strlen(cstring);
  snprintf(cstring + clen, maxlen - clen, fmt, args);
  va_end(args);
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
    char *ptr = (char *)realloc(string->value, capacity);
    if (ptr == NULL) {
      return (String *)NULL;
    }
    string->value = ptr;
    string->capacity = capacity;
  }
  memcpy(string->value, cstring + start, length);
  string->value[length] = '\0';
  string->size = length + 1;
  return string;
}
void string_reverse(char *cstr) {
  int size = strlen(cstr);
  if (size < 2)
    return;
  char ctmp;
  for (int i = 0; i < size / 2; i++) {
    ctmp = cstr[size - 1 - i];
    cstr[size - 1 - i] = cstr[i];
    cstr[i] = ctmp;
  }
}
void strip_mark(String *string) {
  if (!string->value) {
    return;
  }
  int start = sequence(string->value, '"', 0);
  if (start == -2) {
    string_empty(string);
    return;
  }
  int end = sequence(string->value, '"', 1);
  if (end == -2) {
    string_empty(string);
    return;
  }

  if (start == -1 && end == string->size) {
    return;
  }
  int size = (end - start) + 1;
  memcpy(string->value, string->value + start, size);
  string->value[size] = '\0';
  string->size = size + 1;
}
int sequence(char *cstring, char c, int reverse) {
  int size = strlen(cstring);
  int offset = (size - 1) * reverse;
  int iter = 1 + (reverse * -2);
  int j = 0;
  for (int i = 0; i < size; i++) {
    if (cstring[offset + j] != c) {
      return offset + j;
    }
    j += iter;
  }
  return -2;
}
int toInt(String *str) {
  int value = 0;
  for (int i = 0; i < str->size - 1; i++) {
    value *= 10;
    value += (str->value[i] - '0');
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
  if (!string->value) {
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
    if (str->value[i]) {
      printf("%c", str->value[i]);
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
  str->value[0] = '\0';
  str->size = 1;
}
int isEmpty(String *str) { return str->size == 1; }
void free_string(String *string) {
  free(string->value);
  free(string);
}
