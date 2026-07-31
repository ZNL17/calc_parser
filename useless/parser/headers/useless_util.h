#ifndef USELESS_UTIL_H
#define USELESS_UTIL_H
#include "new_string.h"
#include <string.h>
int isNumber(char c);
void reverseCStr(char *cstr);
char *rstrcpy(char *dest, const char *src);
void *rmemcpy(void *destpp, void *srcpp, size_t n);
char decToHex(int d);
String toHex(char c);
int findC(char *cstr, char c);
int lastC(char *cstr, char c);
void printdebug(char *c);
#endif
#define HEX 16
