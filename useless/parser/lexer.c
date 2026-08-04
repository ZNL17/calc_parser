#include "headers/expression.h"
#include "headers/new_string.h"
#include "headers/useless_util.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum { INTEGER, FLOAT, OPERATOR, PARENTHESES, WHITESPACE } Type;
typedef struct {
  int start;
  int end;
} Span;
typedef struct {
  Span span;
  Type type;
} Token;

typedef struct {
  int size;
  int capacity;
  Token *tokens;
} Array;
int whiteList(char c);
int consumeWhiteSpace(Array *array, char **str, int *index);
int consumerNumber(Array *array, char **str, int *index);
int isRightSideFloat(char *str, int *index);
int isWhiteSpace(char c);
int isParentheses(Array *array, char c, int index);
int isSeperator(char c);
void appendToken(Array *array, Token *token);
int isOperator(Array *array, char c, int index);
int getSpanSize(Span span);
void printToken(char *str, Array *array);
int main(int argc, char *argv[]) {
  FILE *ptr;
  ptr = fopen(argv[1], "r");
  if (ptr == NULL) {
    return EXIT_FAILURE;
  }
  int BUFFER_SIZE = 4096;
  char buffer[BUFFER_SIZE];
  while (fgets(buffer, BUFFER_SIZE, ptr)) {
  }
  return EXIT_SUCCESS;
}
void parser(char *str) {
  Array *array;
  int i = 0;
  while (*str) {
    if (!whiteList(*str)) {
      goto Next;
    }
    if (consumeWhiteSpace(array, &str, &i)) {
      break;
    }
    if (isOperator(array, *str, i)) {
      goto Next;
    }
    if (isParentheses(array, *str, i)) {
      goto Next;
    }
    if (consumerNumber(array, &str, &i)) {
      break;
    }
    continue;
  Next:
    str++;
    i++;
  }
}
int whiteList(char c) {
  if (c == '\x09' || c == '\x32') {
    return 1;
  }
  if (c <= '0' && c <= '9') {
    return 1;
  }
  if (c == '.' || c == ',') {
    return 1;
  }
  if (c == '(' || c == ')') {
    return 1;
  }
  return 0;
}
int consumeWhiteSpace(Array *array, char **str, int *index) {
  if (!isWhiteSpace(**str)) {
    return 0;
  }
  Token token;
  int i = (*index)++;
  (*str)++;
  while (**str) {
    if (!isWhiteSpace(**str)) {
      goto Valid;
    }
    (*str)++;
    (*index)++;
  }
  return 1;
Valid:
  token.type = WHITESPACE;
  token.span.start = i;
  token.span.end = *index - 1;
  appendToken(array, &token);
  return 0;
}
int isWhiteSpace(char c) {
  if (c == '\x32' || c == '\x09') {
    return 1;
  }
  return 0;
}
int isOperator(Array *array, char c, int index) {
  Token token;
  switch (c) {
  case Add:
  case Substract:
  case Multply:
  case Divide:
    token.type = OPERATOR;
    token.span.start = index;
    token.span.end = index;
    appendToken(array, &token);
    return 1;
    break;
  }
  return 0;
}
int isParentheses(Array *array, char c, int index) {
  Token token;
  if (c == '(' || c == ')') {
    token.type = PARENTHESES;
    token.span.start = index;
    token.span.end = index;
    appendToken(array, &token);
  }

  return 0;
}
int consumerNumber(Array *array, char **str, int *index) {
  if (!isNumber(**str)) {
    return 0;
  }
  Token token;
  if (**str == '0' && !isSeperator(*(*str + 1))) {
    token.type = INTEGER;
    token.span.start = (*index);
    token.span.end = (*index)++;
    appendToken(array, &token);
    (*str)++;
    return 0;
  }
  int i = (*index)++;
  while (**str) {
    if (!isNumber(**str)) {
      if (**str == '.' || **str == ',') {
        str++;
        if (isRightSideFloat(*str, index)) {
          token.type = FLOAT;
          goto Valid;
        }
        return 0;
      }
      token.type = INTEGER;
      goto Valid;
    }
    (*str)++;
    (*index)++;
  }
  token.type = INTEGER;
Valid:
  token.span.start = i;
  token.span.end = *index - 1;
  appendToken(array, &token);
  return 1;
}
//^(0|\d+)$
int isInteger(Array *array, char *str, int index) {}
//^(0.\d+|\d.\d+)$
int isRightSideFloat(char *str, int *index) {
  if (!isNumber(*str)) {
    return 0;
  }
  str++;
  int i = *index;
  for (; *str; i++) {
    if (!isNumber(*str)) {
      return 1;
    }
    str++;
  }
  return 1;
}
int isSeperator(char c) {
  if (c == '.' || c == ',') {
    return 1;
  }
  return 0;
}
void appendToken(Array *array, Token *token) {
  if (array->size + 1 > array->capacity) {
    int capacity = roundCapacity(array->size + 1);
    Token *ptr = (Token *)realloc(array->tokens, sizeof(Token) * capacity);
    if (ptr == NULL) {
      return;
    }
    array->tokens = ptr;
    array->capacity = capacity;
  }
  memcpy(array->tokens + array->size, token, sizeof(Token));
  array->size++;
}
String *getToken(Type type) {
  String *str = createStr("<UNKNOWN>");
  switch (type) {
  case INTEGER:
    appendCStr(str, "<INTEGER>");
    break;
  case FLOAT:
    appendCStr(str, "<FLOAT>");
    break;
  case OPERATOR:
    appendCStr(str, "<OPERATOR>");
    break;
  case PARENTHESES:
    appendCStr(str, "<PARENTHESES>");
    break;
  case WHITESPACE:
    appendCStr(str, "<WHITESPACE>");
    break;
  }
  return str;
}
int getSpanSize(Span span) { return (span.end - span.start) + 1; }
void printToken(char *str, Array *array) {
  for (int i = 0; i < array->size; i++) {
    Token token = array->tokens[i];
    printf("%s", getToken(token.type)->str);
    printf("<%.*s>", getSpanSize(token.span), str + token.span.start);
  }
}
