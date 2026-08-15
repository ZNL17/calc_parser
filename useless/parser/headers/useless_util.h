#ifndef USELESS_UTIL_H
#define USELESS_UTIL_H
#include "expression.h"
#include "new_string.h"
#include <string.h>
int roundCapacity(int s);
int isNumber(char c);
void reverseCStr(char *cstr);
char *rstrcpy(char *dest, const char *src);
void *rmemcpy(void *destpp, void *srcpp, size_t n);
char decToHex(int d);
String toHex(char c);
int findC(char *cstr, char c);
int lastC(char *cstr, char c);
void printdebug(char *c);
Number whichNumber(String string);
#endif
#define HEX 16
