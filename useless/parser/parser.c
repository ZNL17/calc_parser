#include "headers/new_string.h"
#include "headers/useless_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ADD 43
#define SUBSTRACT 45
#define MULTPLY 42
#define DIVIDE 47
#define LEFT 0
#define RIGHT 1
typedef enum {
  Add = ADD,
  Substract = SUBSTRACT,
  Multply = MULTPLY,
  Divide = DIVIDE
} Operator;
typedef struct _Node Node;
struct _Node {
  Node *parent;
  int value;
  Operator op;
  Node *children[2];
};
Node *CNode();
void addChildNode(Node *parent, int value, int direction);
int isOperator(Node *node, char c);
int operatorCmp(Operator curr, Operator other);
char getOperator(Operator op);
int getPriority(Operator op);
void freeList(Node *node);
void freeNode(Node *node);
int calc(Node *node);
int calcOp(int a, int b, Operator op);
int main(int argc, char *argv[]) {
  if (argc != 2)
    return EXIT_FAILURE;
  char *content = argv[1];
  char *copy = content;
  while (*copy) {
    if (*copy < ' ' || *copy == 127) {
      printf("%s", toHex(*copy).str);
      copy++;
      continue;
    }
    printf("%c", *copy);
    copy++;
  }
  printf("%s\n", content);
  String currNum = createStr("");
  Node *headNode;
  Node *tailNode;
  printf("headNode: %ld\n", (long int)&headNode);
  printf("tailNode: %ld\n", (long int)&tailNode);
  printf("\nstart\n");
  int i = 0;

  printf("int i: %ld\n", (long int)&i);
  while (*content) {
    char currChar = *content;
    printf("char: %ld\n", (long int)&currChar);
    printf("%d>_______\n", i);
    printf("curr(%d): %c\n", i, currChar);
    if (currChar == ' ') {
      printf("index %d is whitespace\n", i);
      goto Next;
    }
    if (isNumber(currChar)) {
      printf("%c is a digit\n", currChar);
      printf("append %c to %s\n", currChar, currNum.str);
      appendChar(&currNum, currChar);
      printf("\ncurrent num: %s\n", currNum.str);
      goto Next;
    }
    Node *currNode = CNode();
    printf("currNode: %ld\n", (long int)&currNode);
    if (!isOperator(currNode, currChar)) {
      goto Fail;
    }
    printf("add child\n");
    printf("before:\n");
    printStr(&currNum);
    addChildNode(currNode, toInt(&currNum), LEFT);
    setStrEmpty(&currNum);
    printf("after:\n");
    printStr(&currNum);
    if (tailNode == NULL) {
      headNode = currNode;
      tailNode = currNode;
      goto Next;
    }
    Node *prev = tailNode;
    printf("prev: %ld\n", (long int)&currNode);
    while (prev != NULL) {
      printf("\nloop");
      int cmp = operatorCmp(currNode->op, prev->op);
      printf("int cmp: %ld\n", (long int)&currNode);
      if (cmp > 0) {
        currNode->children[LEFT] = prev->children[RIGHT];
        currNode->children[LEFT]->parent = currNode;
        prev->children[RIGHT] = currNode;
        currNode->parent = prev;
        if (prev->parent == NULL) {
          headNode = prev;
        }
        tailNode = currNode;
        break;
      } else if (cmp == 0) {
        prev->children[RIGHT] = currNode;
        currNode->parent = prev;
        if (prev->parent == NULL) {
          headNode = prev;
        }
        tailNode = currNode;
        break;
      }
      prev = prev->parent;
    }
    currNode->children[RIGHT] = headNode;
    headNode->parent = currNode;
  Next:
    content++;
    i++;
    printf("%d>_______\n", i);
    printf("next\n");
    continue;
  }
  printf("end\n");
  if (!isEmpty(&currNum)) {
    printf("added child");
    addChildNode(tailNode, toInt(&currNum), RIGHT);
  }
  freeStr(&currNum);
  printf("\nresult: %d", calc(headNode));
  return EXIT_SUCCESS;
Fail:
  printf("ENTERED INCORRECT SYMBOLS");
  return EXIT_FAILURE;
}
int calc(Node *node) {
  if (node->children[LEFT] == NULL) {
    printf("(is value)value: %d, operator: %c\n", node->value,
           getOperator(node->op));

    return node->value;
  }
  printf("(is operation )value: %d, operator: %c\n", node->value,
         getOperator(node->op));
  return calcOp(calc(node->children[LEFT]), calc(node->children[RIGHT]),
                node->op);
}
int calcOp(int a, int b, Operator op) {
  printf("op: %d %c %d\n", a, getOperator(op), b);
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

int isOperator(Node *node, char c) {
  switch (c) {
  case Add:
    node->op = Add;
    return 1;
    break;
  case Substract:
    node->op = Substract;
    return 1;
    break;
  case Multply:
    node->op = Multply;
    return 1;
    break;
  case Divide:
    node->op = Divide;
    return 1;
    break;
  }
  return 0;
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

Node *CNode() { return (Node *)malloc(sizeof(Node)); }
void addChildNode(Node *parent, int value, int direction) {
  Node *node = CNode();
  node->value = value;
  node->parent = parent;
  parent->children[direction] = node;
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
  freeNode(node->children[RIGHT]);
  free(node->children[LEFT]);
}
