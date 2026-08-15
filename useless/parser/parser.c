#include "headers/parser.h"
#include "headers/expression.h"
#include "headers/lexer.h"
#include "headers/stack.h"
#include "headers/useless_util.h"
#include <stdio.h>
#include <stdlib.h>
#define PRINTF
int main(int argc, char *argv[]) {
  if (argc != 2) {
    return EXIT_FAILURE;
  }
  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    return EXIT_FAILURE;
  }
  int win = 0;
  int loss = 0;
  int BUFFER_SIZE = 4096;
  char buffer[BUFFER_SIZE];
  while (fgets(buffer, BUFFER_SIZE, file)) {
    StringList *rows = split(buffer, ',', -1);
    if (rows->size < 3) {
      continue;
    }
    if (!isNumber(rows->strings[2]->string[0])) {
      continue;
    }
    strip_mark(rows->strings[1]);
    Tokens *tokens = lexer(rows->strings[1]->string);
    printTokens(tokens);
    Number result = parse(tokens);
    printf("result: ");
    printNumber(result);
    printf("\n");
    Number expect = whichNumber(*(rows->strings[2]));
    if (equal(result, expect)) {
      win++;
      continue;
    }
    loss++;
    printf("assert failed for %s\n", rows->strings[1]->string);
    printf("expected %s\n", rows->strings[2]->string);
    printf("given ");
    printNumber(result);
  }
  printf("tests: %d,win: %d, loss: %d\n", win + loss, win, loss);
  printf("pass rate: %d\%\n", (win / (win + loss)) * 100);
  fclose(file);
}
Number parse(Tokens *tokens) {
  Node *headNode = NULL;
  Node *tailNode = NULL;
  Stack *stack = CStack();
  int i = 0;
  Token currNum = (Token){UNKNOWN, NULL};
  for (; i < tokens->size; i++) {
    Token token = tokens->tokens[i];
    if (token.type == PARENTHESES && isTokenChar(token, '(')) {
      PRINTF("(stack size: %d\n", stack->size);
      State state = CState(headNode, tailNode);
      if (state.head == NULL || state.tail == NULL) {
        printf("why \n");
      }
      push(stack, &state);
      headNode = NULL;
      tailNode = NULL;
      currNum = (Token){UNKNOWN, NULL};
      continue;
    }
    if (token.type == PARENTHESES && isTokenChar(token, ')')) {
      if (currNum.type != UNKNOWN) {
        PRINTF("last ) add\n");
        // printNode(tailNode, "add last to tailNode");
        addChildNode(tailNode, currNum, RIGHT);
        currNum = (Token){UNKNOWN, NULL};
      }
      PRINTF(")stack size: %d\n", stack->size);
      State state = pop(stack);
      if (state.head == NULL) {
        PRINTF("yolo\n");
      }
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
    if (token.type == INTEGER || token.type == FLOAT) {
      PRINTF("int\n");
      currNum = token;
      continue;
    }
    Node *currNode = CNode();
    currNode->value = token;
    PRINTF("add childnode\n");
    addChildNode(currNode, currNum, LEFT);
    PRINTF("add op to tree\n");
    if (tailNode == NULL) {
      PRINTF("add first node\n");
      headNode = currNode;
      tailNode = currNode;
      continue;
    }
    PRINTF("add\n");
    addExprToTree(currNode, &headNode, &tailNode);
    PRINTF("-------------------------------------\n");
  }
  if (currNum.type != UNKNOWN) {
    PRINTF("add last num to tree\n");
    // printNode(tailNode, "add last to tailNode");
    addChildNode(tailNode, currNum, RIGHT);
    PRINTF("after added last \n");
  }
  PRINTF("calc\n");
  // freeStr(&currNum);
  return calc(headNode);
Fail:
  PRINTF("ENTERED INCORRECT SYMBOLS\n");
  return (Number){0, {}};
}
State CState(Node *head, Node *tail) {
  State state;
  state.head = head;
  state.tail = tail;
  return state;
}
