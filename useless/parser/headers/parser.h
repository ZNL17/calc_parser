#ifndef PARSER_H
#define PARSER_H
#include "expression.h"
#include "lexer.h"
#include "new_string.h"
typedef struct {
  Node *head;
  Node *tail;
} State;
Number parse(Tokens *tokens);
State CState(Node *head, Node *tail);
#endif
