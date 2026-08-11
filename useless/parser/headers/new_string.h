#ifndef NEW_STRING_H
#define NEW_STRING_H
typedef struct {
  char *string;
  int size;
  int capacity;
} String;
typedef struct {
  int size;
  int capacity;
  String **strings;
} StringList;
String *string_add_value_new(char *cstring);
String *string_new();
void string_set_cstrings(String *string, char *cstring);
void string_empty(String *string);
// substring gives a partial string from your input string
// input "012345", start = 1, end = 3
// output "12"
// start is inclusive
// end is exclusive
String *substring(char *cstring, int start, int end);
void strip_mark(String *string);
int sequence(char *string, char c, int reverse);
void appendCStr(String *str, char *content);
void appendStr(String *str, String *otherStr);
void appendChar(String *str, char c);
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
