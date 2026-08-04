#include "headers/parser.h"
#include "headers/expression.h"
#include "headers/new_string.h"
#include "headers/stack.h"
#include "headers/useless_util.h"
#include <stdio.h>
#include <stdlib.h>
#define PRINTF printf
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
  Stack *stack = CStack();
  int i = 0;
  while (*content) {
    char currChar = *content;
    PRINTF("(%d) >>> current char: (%c)\n", i + 1, currChar);
    if (currChar == ' ' || currChar == '"') {
      PRINTF("index %d is whitespace\n", i);
      goto Next;
    }
    if (currChar == '(') {
      State state = CState(headNode, tailNode);
      push(stack, &state);
      headNode = NULL;
      tailNode = NULL;
      continue;
    }
    if (currChar == ')') {
      State state = pop(stack);
      if (tailNode == NULL) {
        state.head = headNode;
        state.tail = tailNode;
        continue;
      }
      state.tail->children[RIGHT] = headNode;
      headNode = state.head;
      tailNode = state.tail;
      continue;
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
State CState(Node *head, Node *tail) {
  State state;
  state.head = head;
  state.tail = tail;
  return state;
}
