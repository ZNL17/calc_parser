#include "headers/lexer.h"
#include "headers/expression.h"
#include "headers/new_string.h"
#include "headers/useless_util.h"
#include <stdio.h>
#include <stdlib.h>
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
    printf("row size(%d): \n", rows->size);
    if (rows->size < 3) {
      printf("skip line\n");
      continue;
    }
    strip_mark(rows->strings[1]);
    printf("input: %s: \n", rows->strings[1]->value);
    Tokens *tokens = lexer(rows->strings[1]->value);
    printf("tokens size(%d): \n", tokens->size);
    printTokens(tokens);
    printf("\niter end\n");
  }
  return EXIT_SUCCESS;
}
Tokens *lexer(char *str) {
  Tokens *tokens = array_new();
  int i = 0;
  while (*str) {
    if (!whiteList(*str)) {
      goto Next;
    }
    if (isWhiteSpace(*str)) {
      goto Next;
    }
    if (isOperator(tokens, *str, i)) {
      goto Next;
    }
    if (isParentheses(tokens, *str, i)) {
      goto Next;
    }
    if (consumeInteger(tokens, &str, &i)) {
      goto Next;
    }
    if (consumeFloat(tokens, &str, &i)) {
      goto Next;
    }
  Next:
    str++;
    i++;
  }
  return tokens;
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
int consumeWhiteSpace(Tokens *array, char **str, int *index) {
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
  if (c == SPACE || c == HORIZONTAL_TAB) {
    return 1;
  }
  return 0;
}
int isOperator(Tokens *tokens, char c, int index) {
  switch (c) {
  case Add:
  case Substract:
  case Multply:
  case Divide:
    setTokenChar(tokens, OPERATOR, c, index, index + 1);
    return 1;
  }
  return 0;
}
int isParentheses(Tokens *array, char c, int index) {
  if (c == '(' || c == ')') {
    setTokenChar(array, PARENTHESES, c, index, index + 1);
    return 1;
  }
  return 0;
}
int consumeInteger(Tokens *tokens, char **str, int *index) {
  if (!isNumber(**str)) {
    return 0;
  }
  char *startChar = (*str)++;
  int startIndex = (*index)++;
  for (; **str; (*str)++) {
    if (!isNumber(**str)) {
      if (**str == '.') {
        *str = startChar;
        *index = startIndex;
        return 0;
      }
      break;
    }
    (*index)++;
  }
  setToken(tokens, INTEGER, *str - *index, startIndex, *index);
  (*str)--;
  (*index)--;
  return 1;
}
//^-?\d+\.\d+$
int consumeFloat(Tokens *tokens, char **str, int *index) {
  if (!isNumber(**str)) {
    return 0;
  }
  char *startChar = (*str)++;
  int startIndex = (*index)++;
  for (; **str; (*str)++) {
    if (!isNumber(**str)) {
      if (**str != '.') {
        *str = startChar;
        *index = startIndex;
        return 0;
      }
      (*str)++;
      (*index)++;
      break;
    }
    (*index)++;
  }
  for (; **str; (*str)++) {
    if (!isNumber(**str)) {
      break;
    }
    (*index)++;
  }
  setToken(tokens, FLOAT, *str - *index, startIndex, *index);
  (*str)--;
  (*index)--;
  return 1;
}
int consumerNumber(Tokens *array, char **str, int *index) {
  if (!isNumber(**str)) {
    return 0;
  }
  if (**str == '0' && !isSeperator(*(*str + 1))) {
    setToken(array, INTEGER, *str, *index, ++(*index));
    (*str)++;
    return 0;
  }
  int i = *index;
  while (**str) {
    if (!isNumber(**str)) {
      if (**str == '.' || **str == ',') {
        (*str)++;
        (*index)++;
        if (isRightSideFloat(str, index)) {
          setToken(array, FLOAT, *str - *index, i, *index);
          return 0;
        }
        return 0;
      }
      setToken(array, INTEGER, *str - *index, i, *index);
      return 0;
    }
    (*str)++;
    (*index)++;
  }
  setToken(array, INTEGER, *str - *index, i, *index);
  return 1;
}
void setToken(Tokens *array, Type type, char *cstring, int start, int end) {
  String *string = substring(cstring, start, end);
  Span span = {start, end};
  Token token = {type, string, span};
  appendToken(array, &token);
}
void setTokenChar(Tokens *array, Type type, char c, int start, int end) {
  String *string = string_new();
  string_append_char(string, c);
  Span span = {start, end};
  Token token = {type, string, span};
  appendToken(array, &token);
}
//^(0|\d+)$
int isInteger(Tokens *array, char *str, int index) {}
//^(0.\d+|\d.\d+)$
int isRightSideFloat(char **str, int *index) {
  if (!isNumber(**str)) {
    return 0;
  }
  (*str)++;
  (*index)++;
  for (; *str; (*index)++) {
    if (!isNumber(**str)) {
      return 1;
    }
    (*str)++;
  }
  return 1;
}
int isSeperator(char c) {
  if (c == '.' || c == ',') {
    return 1;
  }
  return 0;
}
Tokens *array_new() {
  Tokens *ptr = (Tokens *)malloc(sizeof(Tokens));
  if (ptr == NULL) {
    return (Tokens *)NULL;
  }
  Token *tokens_ptr = (Token *)malloc(sizeof(Token) * 10);
  if (tokens_ptr == NULL) {
    return (Tokens *)NULL;
  }
  ptr->tokens = tokens_ptr;
  ptr->size = 0;
  ptr->capacity = 10;
  return ptr;
}
void appendToken(Tokens *array, Token *token) {
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
  String *token = string_new_value("<UNKNOWN>");
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
void printTokens(Tokens *tokens) {
  for (int i = 0; i < tokens->size; i++) {
    Token token = tokens->tokens[i];
    printf("%s", getToken(token.type)->value);
    printf("<%s>\n", token.string->value);
    // printf("<%.*s>,", getSpanSize(token.span), str + token.span.start);
  }
  printf("\n");
}
int isTokenChar(Token token, char c) { return token.string->value[0] == c; }
