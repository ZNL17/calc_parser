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

Array *parser(char *str);
int whiteList(char c);
int consumeWhiteSpace(Array *array, char **str, int *index);
int consumerNumber(Array *array, char **str, int *index);
int isRightSideFloat(char *str, int *index);
int isWhiteSpace(char c);
int isParentheses(Array *array, char c, int index);
int isSeperator(char c);
Array *array_new();
void appendToken(Array *array, Token *token);
void setToken(Array *array, Type type, int start, int end);
int isOperator(Array *array, char c, int index);
int getSpanSize(Span span);
void printTokens(char *str, Array *array);
int main(int argc, char *argv[]) {
  FILE *ptr;
  ptr = fopen(argv[1], "r");
  if (ptr == NULL) {
    return EXIT_FAILURE;
  }
  int BUFFER_SIZE = 4096;
  char buffer[BUFFER_SIZE];
  while (fgets(buffer, BUFFER_SIZE, ptr)) {
    StringList *rows = split(buffer, ',', -1);
    printf("row(%d): ", rows->size);
    if (rows->size < 3) {
      printf("skip line\n");
      continue;
    }
    strip_mark(rows->strings[1]);
    printf("%s\n", rows->strings[1]->string);
    Array *array = parser(rows->strings[1]->string);
    printf("array(%d): \n", array->size);
    printTokens(rows->strings[1]->string, array);
    printf("iter end\n");
  }
  return EXIT_SUCCESS;
}
Array *parser(char *str) {
  Array *array = array_new();
  int i = 0;
  while (*str) {
    if (!whiteList(*str)) {
      goto Next;
    }
    if (isWhiteSpace(*str)) {
      goto Next;
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
  return array;
}
int whiteList(char c) {
  if (c == '\x09' || c == '\x20') {
    return 1;
  }
  if (c >= '0' && c <= '9') {
    return 1;
  }
  if (c == '.' || c == ',') {
    return 1;
  }
  if (c == '(' || c == ')') {
    return 1;
  }
  if (c == '+' || c == '-' || c == '*' || c == '/') {
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
  if (c == '\x20' || c == '\x09') {
    return 1;
  }
  return 0;
}
int isOperator(Array *array, char c, int index) {
  switch (c) {
  case Add:
  case Substract:
  case Multply:
  case Divide:
    setToken(array, OPERATOR, index, index);
    return 1;
  }
  return 0;
}
int isParentheses(Array *array, char c, int index) {
  Token token;
  if (c == '(' || c == ')') {
    setToken(array, PARENTHESES, index, index);
    return 1;
  }
  return 0;
}
int consumerNumber(Array *array, char **str, int *index) {
  if (!isNumber(**str)) {
    return 0;
  }
  Token token;
  if (**str == '0' && !isSeperator(*(*str + 1))) {
    setToken(array, INTEGER, *index, (*index)++);
    (*str)++;
    return 0;
  }
  int i = *index;
  while (**str) {
    if (!isNumber(**str)) {
      if (**str == '.' || **str == ',') {
        str++;
        if (isRightSideFloat(*str, index)) {
          setToken(array, FLOAT, i, *index - 1);
          return 0;
        }
        return 0;
      }
      setToken(array, INTEGER, i, (*index) - 1);
      return 0;
    }
    (*str)++;
    (*index)++;
  }
  setToken(array, INTEGER, i, *index - 1);
  return 1;
}
void setToken(Array *array, Type type, int start, int end) {
  Span span = {start, end};
  Token token = {span, type};

  appendToken(array, &token);
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
Array *array_new() {
  Array *ptr = (Array *)malloc(sizeof(Array));
  if (ptr == NULL) {
    return (Array *)NULL;
  }
  Token *tokens_ptr = (Token *)malloc(sizeof(Token) * 10);
  if (tokens_ptr == NULL) {
    return (Array *)NULL;
  }
  ptr->tokens = tokens_ptr;
  ptr->size = 0;
  ptr->capacity = 10;
  return ptr;
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
  String *token = string_add_value_new("<UNKNOWN>");
  switch (type) {
  case INTEGER:
    string_set_cstrings(token, "<INTEGER> -> ");
    break;
  case FLOAT:
    string_set_cstrings(token, "<FLOAT> -> ");
    break;
  case OPERATOR:
    string_set_cstrings(token, "<OPERATOR> -> ");
    break;
  case PARENTHESES:
    string_set_cstrings(token, "<PARENTHESES> -> ");
    break;
  case WHITESPACE:
    string_set_cstrings(token, "<WHITESPACE> -> ");
    break;
  }
  return token;
}
int getSpanSize(Span span) { return (span.end - span.start) + 1; }
void printTokens(char *str, Array *array) {
  for (int i = 0; i < array->size; i++) {
    Token token = array->tokens[i];
    printf("%s", getToken(token.type)->string);
    printf("<%.*s>,", getSpanSize(token.span), str + token.span.start);
  }
}
