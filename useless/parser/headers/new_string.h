#ifndef NEW_STRING_H
#define NEW_STRING_H
typedef struct {
  char *str;
  int size;
  int capacity;
} String;
String *createStr(char *str);
void appendCStr(String *str, char *content);
void appendStr(String *str, String *otherStr);
void appendChar(String *str, char c);
int toInt(String *str);
void printNull(String *str);
void printStr(String *str);
void printHidden(char *str);
void setStrEmpty(String *str);
int isEmpty(String *str);
void freeStr(String *str);
#endif
