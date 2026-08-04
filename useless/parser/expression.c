#include "headers/expression.h"
#include <stdio.h>
#include <stdlib.h>
#define PRINTF printf

void addExprToTree(Node *currNode, Node **headNode, Node **tailNode) {
  printNode(currNode, "currNode");
  printNode(*headNode, "headNode");
  printNode(*tailNode, "tailNode");
  int cmp = operatorCmp(currNode->op, (*tailNode)->op);
  PRINTF("cmp\n");
  if (cmp < 1) {
    PRINTF("currNode is equal/lesser\n");
    Node *prev = *tailNode;
    while (prev->parent != NULL) {
      PRINTF("iter start\n");
      if (0 < operatorCmp(currNode->op, prev->parent->op)) {
        PRINTF("found node() that is smaller %c > %c\n",
               getOperator(currNode->op), getOperator(prev->parent->op));
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
int calc(Node *node) {
  if (node == NULL) {
    PRINTF("NULL\n");
    return -1;
  }
  if (node->children[LEFT] == NULL) {
    PRINTF("(value)value: %d\n", node->value);
    return node->value;
  }
  PRINTF("(operation)operator: %c\n", getOperator(node->op));
  return calcOp(calc(node->children[LEFT]), calc(node->children[RIGHT]),
                node->op);
}
int calcOp(int a, int b, Operator op) {
  PRINTF("op: %d %c %d\n", a, getOperator(op), b);
  switch (op) {
  case Add:
    return a + b;
    break;
  case Substract:
    return a - b;
    break;
  case Multply:
    return a * b;
    break;
  case Divide:
    return a / b;
    break;
  }
}

int operatorCmp(Operator curr, Operator other) {
  return getPriority(curr) - getPriority(other);
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
char getOperator(Operator op) {
  switch (op) {
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

Node *CNode() {
  Node *node = (Node *)malloc(sizeof(Node));
  node->parent = NULL;
  node->children[LEFT] = NULL;
  return node;
}
void addChildNode(Node *parent, int value, int direction) {
  Node *node = CNode();
  node->value = value;
  node->parent = parent;
  parent->children[direction] = node;
}

void printNode(Node *node, char *name) {
  PRINTF("%s (%c, %d)\n", name, getOperator(node->op), node->value);
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
