#include "headers/parser.h"
#include "headers/new_string.h"
#include "headers/useless_util.h"
#include <stdio.h>
#include <stdlib.h>
#define PRINTF
int main(int argc, char *argv[]) {
  if (argc != 2) {
    return EXIT_FAILURE;
  }
  int BUFFER_SIZE = 4096;
  char buffer[BUFFER_SIZE];
  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    return EXIT_FAILURE;
  }
  int win = 0;
  int loss = 0;
  // skips header
  fgets(buffer, BUFFER_SIZE, file);
  while (fgets(buffer, BUFFER_SIZE, file)) {
    PRINTF("read: %s", buffer);
    int startpos = findC(buffer, ',') + 1;
    PRINTF("startpos %d\n", startpos);
    int lastpos = lastC(buffer, ',') + 1;
    PRINTF("lastpos %d\n", lastpos);
    int exp = atoi(buffer + lastpos);
    PRINTF("atoi works\n");
    int size = lastpos - startpos;
    char *ptr = (char *)malloc(size);
    if (ptr == NULL) {
      PRINTF("wow no memory?\n");
    }
    memcpy(ptr, buffer + startpos, size);
    ptr[size - 1] = '\0';
    int result = parse(ptr);
    if (result != exp) {
      printf("assertion failed\n");
      printf("expr: %.*s expects %d\n", size - 1, buffer + startpos, exp);
      printf("given %d\n", result);
      loss++;
      continue;
    }
    printf("assertion true\n");
    printf("expr: %.*s expects %d\n", size - 1, buffer + startpos, exp);
    printf("given %d\n", result);
    win++;
    PRINTF("iter end\n");
  }
  printf("total: %d, passed: %d, failed :%d", win + loss, win, loss);
  fclose(file);
}
int parse(char *content) {
  String currNum = createStr("");
  Node *headNode = NULL;
  Node *tailNode = NULL;
  int i = 0;
  while (*content) {
    char currChar = *content;
    PRINTF("(%d) >>> current char: (%c)\n", i + 1, currChar);
    if (currChar == ' ' || currChar == '"') {
      PRINTF("index %d is whitespace\n", i);
      goto Next;
    }
    if (isNumber(currChar)) {
      PRINTF("%c is a digit\n", currChar);
      PRINTF("append %c to %s\n", currChar, currNum.str);
      appendChar(&currNum, currChar);
      PRINTF("\ncurrent num: %s\n", currNum.str);
      goto Next;
    }

    Node *currNode = CNode();
    if (!isOperator(currNode, currChar)) {
      goto Fail;
    }

    PRINTF("add op to tree\n");
    addChildNode(currNode, toInt(&currNum), LEFT);
    setStrEmpty(&currNum);
    if (tailNode == NULL) {
      PRINTF("add first node\n");
      headNode = currNode;
      tailNode = currNode;
      goto Next;
    }
    PRINTF("add\n");
    addExprToTree(currNode, &headNode, &tailNode);
    PRINTF("\nresult: %d", calc(headNode));
    PRINTF("-------------------------------------");
  Next:
    PRINTF("(%d) >>> end of iter\n", i + 1);
    content++;
    i++;
    continue;
  }
  if (!isEmpty(&currNum)) {
    PRINTF("add last num to tree\n");
    printNode(tailNode, "add last to tailNode");
    addChildNode(tailNode, toInt(&currNum), RIGHT);
  }
  freeStr(&currNum);
  return calc(headNode);
Fail:
  PRINTF("ENTERED INCORRECT SYMBOLS\n");
  return EXIT_FAILURE;
}
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
  freeNode(node->children[RIGHT]);
  free(node->children[LEFT]);
}
