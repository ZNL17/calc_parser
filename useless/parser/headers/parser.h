#ifndef PARSER_H
#define PARSER_H
#include "expression.h"
typedef struct {
  Node *head;
  Node *tail;
} State;
int parse(char *content);
State CState(Node *head, Node *tail);
#endif
