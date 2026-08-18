#ifndef PARSER_H
#define PARSER_H
#include "expression.h"
#include "lexer.h"
#include "new_string.h"
typedef struct {
  Node *head;
  Node *tail;
} State;
typedef struct {
  Node **node;
  int size;
  int capacity;
} NodeList;
Node *parse(Tokens *tokens);
State CState(Node *head, Node *tail);
NodeList *nodeList_new();
void nodeList_append(NodeList *nodeList, Node *node);
Node *recursivParse(Tokens *tokens, int *index, int level);
void printNodes(Node *node);
#endif
