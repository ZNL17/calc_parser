#include "headers/expression.h"
#include "headers/lexer.h"
#include "headers/new_string.h"
#include <stdio.h>
#include <stdlib.h>
#define PRINTF printf

void addExprToTree(Node *currNode, Node **headNode, Node **tailNode) {
  printNode(currNode, "currNode");
  printNode(*headNode, "headNode");
  printNode(*tailNode, "tailNode");
  int cmp = operatorCmp(whichOperator(currNode->value),
                        whichOperator((*tailNode)->value));
  PRINTF("cmp\n");
  if (cmp < 1) {
    PRINTF("currNode is equal/lesser\n");
    Node *prev = *tailNode;
    while (prev->parent != NULL) {
      PRINTF("iter start\n");
      if (0 < operatorTokenCmp(currNode->value, prev->parent->value)) {
        PRINTF("found node() that is smaller %c > %c\n", getOperator(*currNode),
               getOperator(*prev->parent));
        PRINTF("break\n");
        break;
      }
      prev = prev->parent;
      PRINTF("switch to next iter\n");
    }
    PRINTF("finished search\n");
    if (prev->parent == NULL) {
      printNode(prev, "prev-parent is null this is prev");
      *headNode = currNode;
    } else {
      printNode(prev->parent, "prev-parent is not head");
      prev->parent->children[RIGHT] = currNode;
      currNode->parent = prev->parent;
    }
    PRINTF("add to tail\n");
    (*tailNode)->children[RIGHT] = currNode->children[LEFT];
    (*tailNode)->children[RIGHT]->parent = *tailNode;
    PRINTF("add to left\n");
    currNode->children[LEFT] = prev;
    prev->parent = currNode;
    *tailNode = currNode;
    PRINTF("return < 1\n");
    return;
  }
  PRINTF("currNode is greater\n");
  (*tailNode)->children[RIGHT] = currNode;
  currNode->parent = *tailNode;
  *tailNode = currNode;
  PRINTF("return > 0\n");
}
Number specifyNumber(Token value) {
  switch (value.type) {
  case INTEGER:
    return (Number){INTEGER, {.integer = atoi(value.string->value)}};
  case FLOAT:
    return (Number){FLOAT, {.decimal = atof(value.string->value)}};
  }
  return (Number){0, {}};
}
String *fmtNumber(Number number) {
  String *fmt = string_new();
  switch (number.type) {
  case INTEGER:
    string_append_cstring(fmt, "%d");
    return fmt;
  case FLOAT:
    string_append_cstring(fmt, "%f");
    return fmt;
  }
  return fmt;
}
Number calc(Node *node) {
  if (node->children[LEFT] == NULL) {
    return specifyNumber(node->value);
  }
  return calcOp(calc(node->children[LEFT]), calc(node->children[RIGHT]), *node);
}
Number calcOp(Number a, Number b, Node node) {
  PRINTF("op: ");
  printNumber(a);
  printf(" %c ", getOperator(node));
  printNumber(b);
  printf("\n");
  switch (getOperator(node)) {
  case Add:
    PRINTF("+");
    return add(a, b);
  case Substract:
    PRINTF("-");
    return substract(a, b);
  case Multply:
    PRINTF("*");
    return multiply(a, b);
  case Divide:
    PRINTF("/");
    return divide(a, b);
  }
  return (Number){0, {}};
}
float intToFloat(Number a) {
  if (a.type == FLOAT) {
    return a.numeric.decimal;
  }
  return (float)a.numeric.integer;
}
Number add(Number a, Number b) {
  if (a.type + b.type > 2) {
    return (Number){.type = FLOAT, {.decimal = intToFloat(a) + intToFloat(b)}};
  }
  return (Number){.type = INTEGER,
                  {.integer = a.numeric.integer + b.numeric.integer}};
}

Number substract(Number a, Number b) {
  if (a.type + b.type > 2) {
    return (Number){.type = FLOAT, {.decimal = intToFloat(a) - intToFloat(b)}};
  }
  return (Number){.type = INTEGER,
                  {.integer = a.numeric.integer - b.numeric.integer}};
}
Number multiply(Number a, Number b) {
  if (a.type + b.type > 2) {
    return (Number){.type = FLOAT, {.decimal = intToFloat(a) * intToFloat(b)}};
  }
  return (Number){.type = INTEGER,
                  {.integer = a.numeric.integer * b.numeric.integer}};
}
Number divide(Number a, Number b) {
  if ((a.type + b.type) > (INTEGER + INTEGER)) {
    return (Number){.type = FLOAT, {.decimal = intToFloat(a) / intToFloat(b)}};
  }
  return (Number){.type = INTEGER,
                  {.integer = a.numeric.integer / b.numeric.integer}};
}
int equal(Number a, Number b) {
  switch (a.type + b.type) {
  case INTEGER + INTEGER:
    return a.numeric.integer == b.numeric.integer;
  case FLOAT + FLOAT:
    return a.numeric.decimal == b.numeric.decimal;
  case INTEGER + FLOAT:
    return intToFloat(a) == intToFloat(b);
  }
  return 0;
}
int operatorCmp(Operator curr, Operator other) {
  return getPriority(curr) - getPriority(other);
}
int operatorTokenCmp(Token curr, Token other) {
  return getPriority(whichOperator(curr)) - getPriority(whichOperator(other));
}
int getPriority(Operator op) {
  switch (op) {
  case Add:
    return 0;
  case Substract:
    return 0;
  case Multply:
    return 1;
  case Divide:
    return 1;
  }
  return -1;
}
char getOperator(Node node) {
  switch (whichOperator(node.value)) {
  case Add:
    return '+';
  case Substract:
    return '-';
  case Multply:
    return '*';
  case Divide:
    return '/';
  }
  return '\0';
}
Operator whichOperator(Token token) {
  if (token.type != OPERATOR) {
    return 0;
  }
  if (!*token.string->value) {
    return 0;
  }
  switch (*token.string->value) {
  case '+':
    return Add;
  case '-':
    return Substract;
  case '*':
    return Multply;
  case '/':
    return Divide;
  }
  return 0;
}

Node *CNode() {
  Node *node = (Node *)malloc(sizeof(Node));
  node->parent = NULL;
  node->children[LEFT] = NULL;
  return node;
}
void addChildNode(Node *parent, Node *value, int direction) {
  parent->children[direction] = value;
}
int number_len(Number n) {}
void printNumber(Number n) {
  if (n.type == FLOAT) {
    printf("%f", n.numeric.decimal);
    return;
  }
  printf("%d", n.numeric.integer);
}
void printNode(Node *node, char *name) {
  if (node == NULL) {
    printf("is Null\n");
    return;
  }
  printf("%s (%c)\n", name, getOperator(*node));
  // printNode(node->children[LEFT], "node>");
  // printNode(node->children[RIGHT], "node>");
}
void freeList(Node *node) {
  Node *curr = node;
  while (!curr) {
    Node *next = curr->parent;
    if (next == NULL)
      break;
    curr = next;
  }
  freeNode(curr);
}
void freeNode(Node *node) {
  if (node->children[LEFT] == NULL) {
    return;
  }
  freeNode(node->children[LEFT]);
  free(node->children[LEFT]);
  freeNode(node->children[RIGHT]);
  free(node->children[RIGHT]);
}
