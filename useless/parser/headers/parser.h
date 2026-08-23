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
void parse(char **argv);
void fileParse(char **argv);
Node *recursivParse(Tokens *tokens, int *index, int level);
int negation_rule(Tokens *tokens);
State CState(Node *head, Node *tail);
NodeList *nodeList_new();
void nodeList_append(NodeList *nodeList, Node *node);
void printNodes(Node *node);
#endif
