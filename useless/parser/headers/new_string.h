#ifndef NEW_STRING_H
#define NEW_STRING_H
#include <string.h>
typedef struct {
  char *value;
  int size;
  int capacity;
} String;
typedef struct {
  int size;
  int capacity;
  String **strings;
} StringList;
String *string_new();
String *string_new_value(char *cstring);
void string_append_cstring(String *str, char *content);
void string_append_char(String *str, char c);
void string_set_cstrings(String *string, char *cstring);
void string_empty(String *string);

void string_append_fmt_string(String *string, const char *fmt, ...);

void string_append_fmt_cstring(char *cstring, size_t maxlen, const char *fmt,
                               ...);
// substring gives a partial string from your input string
// input "012345", start = 1, end = 3
// output "12"
// start is inclusive
// end is exclusive
String *substring(char *cstring, int start, int end);
void string_reverse(char *cstr);
void strip_mark(String *string);
int sequence(char *string, char c, int reverse);
int toInt(String *str);
StringList *cStringList();
StringList *split(char *cstring, char c, int count);
void stringList_append(StringList *stringList, String *string);
void freeStringList(StringList *stringList);
void printNull(String *str);
void printStr(String *str);
void printHidden(char *str);
void setStrEmpty(String *str);
int isEmpty(String *str);
void free_string(String *string);
#endif
