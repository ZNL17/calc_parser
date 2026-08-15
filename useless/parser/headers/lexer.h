#ifndef LEXER_H
#define LEXER_H
#include "new_string.h"
#define UNKNOWN 0
typedef enum {
  INTEGER = 1,
  FLOAT = 2,
  OPERATOR = 3,
  PARENTHESES = 4,
  WHITESPACE = 5
} Type;
typedef struct {
  int start;
  int end;
} Span;
typedef struct {
  Type type;
  String *string;
  Span span;
} Token;

typedef struct {
  int size;
  int capacity;
  Token *tokens;
} Tokens;

Tokens *lexer(char *str);
int whiteList(char c);
int consumeWhiteSpace(Tokens *array, char **str, int *index);
int consumerNumber(Tokens *array, char **str, int *index);
int isRightSideFloat(char **str, int *index);
int isWhiteSpace(char c);
int isParentheses(Tokens *array, char c, int index);
int isSeperator(char c);
Tokens *array_new();
void appendToken(Tokens *array, Token *token);
void setToken(Tokens *array, Type type, char *cstring, int start, int end);
void setTokenChar(Tokens *array, Type type, char c, int start, int end);
int isOperator(Tokens *array, char c, int index);
int getSpanSize(Span span);
void printTokens(Tokens *tokens);
int isTokenChar(Token token, char c);
#endif
