#include "headers/parser.h"
#include "headers/expression.h"
#include "headers/lexer.h"
#include "headers/new_string.h"
#include "headers/stack.h"
#include "headers/useless_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
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
  String *error = string_new();
  int BUFFER_SIZE = 4096;
  char buffer[BUFFER_SIZE];
  NodeList *nodeList = nodeList_new();
  while (fgets(buffer, BUFFER_SIZE, file)) {
    StringList *rows = split(buffer, ',', -1);
    if (rows->size < 3) {
      continue;
    }
    if (!isNumber(rows->strings[2]->value[0])) {
      continue;
    }
    strip_mark(rows->strings[1]);
    Tokens *tokens = lexer(rows->strings[1]->value);
    int i = 0;
    Node *headNode = recursivParse(tokens, &i, 0);
    if (headNode == NULL) {
      printf(" isNull\n");
      return 0;
    }
    Number result = calc(headNode);
    printf("result: ");
    printNumber(result);
    Number expect = whichNumber(*(rows->strings[2]));
    if (equal(result, expect)) {
      printf("win\n");
      win++;
      continue;
    }
    nodeList_append(nodeList, headNode);
    printf("loss\n");
    loss++;
    string_append_fmt_string(error, "assert failed for %s\n",
                             rows->strings[1]->value);
    printf("w_%s", rows->strings[2]->value);
    string_append_fmt_string(error, "expected %s", rows->strings[2]->value);
    String *given = string_new_value("given ");
    string_append_cstring(given, fmtNumber(result)->value);
    string_append_cstring(given, "\n");
    string_append_fmt_string(error, given->value, result);
    string_append_cstring(error, "-------------------\n");
  }
  printf("tests: %d,win: %d, loss: %d\n", win + loss, win, loss);
  printf("pass rate: %d\%\n", win * 100 / (win + loss));
  printf("%s", error->value);
  printf("dd: %d", nodeList->size);
  for (int i = 0; i < nodeList->size; i++) {
    printf("%s\n", nodeList->node[i]->value.string->value);
    printf("%lu\n", (unsigned long)nodeList->node[i]);
    printNode(nodeList->node[i], "node>");
  }
  fclose(file);
}
Node *recursivParse(Tokens *tokens, int *index, int level) {
  printf("level: %d\n", level);
  Node *headNode = NULL;
  Node *tailNode = NULL;
  Node *leftNode = NULL;
  for (; *index < tokens->size; (*index)++) {
    printf("size:%d\n", tokens->size);
    printf("index:\n", *index);
    Token token = tokens->tokens[*index];
    printf("token(%s)\n", token.string->value);
    if (token.type == PARENTHESES && isTokenChar(token, '(')) {
      (*index)++;
      printf("(\n");
      leftNode = recursivParse(tokens, index, ++level);
      continue;
    }
    if (token.type == PARENTHESES && isTokenChar(token, ')')) {
      printf(")%s\n", tailNode->value.string->value);
      addChildNode(tailNode, leftNode, RIGHT);
      printf("level: %d\n", level);
      printf(")added\n");
      return headNode;
    }
    if (token.type == INTEGER || token.type == FLOAT) {
      leftNode = CNode();
      leftNode->value = token;
      continue;
    }
    Node *currNode = CNode();
    currNode->value = token;
    addChildNode(currNode, leftNode, LEFT);
    if (tailNode == NULL) {
      PRINTF("current node empty\n");
      headNode = currNode;
      tailNode = currNode;
      printf("tailNode == NULL\n");
      continue;
    }
    addExprToTree(currNode, &headNode, &tailNode);
    printf("addExprToTree\n");
  }
  if (tailNode == NULL && leftNode != NULL) {
    return leftNode;
  }
  addChildNode(tailNode, leftNode, RIGHT);
  return headNode;
}
Node *parse(Tokens *tokens) {
  Node *headNode = NULL;
  Node *tailNode = NULL;
  Node *leftNode = NULL;
  Stack *stack = CStack();
  int i = 0;
  Token currNum = (Token){UNKNOWN, NULL};
  for (; i < tokens->size; i++) {
    Token token = tokens->tokens[i];
    if (token.type == PARENTHESES && isTokenChar(token, '(')) {
      printNodes(headNode);
      PRINTF("(stack size: %d\n", stack->size);
      State state = CState(headNode, tailNode);
      push(stack, &state);
      printf("push success state\n");
      headNode = NULL;
      tailNode = NULL;
      currNum = (Token){UNKNOWN, NULL};
      continue;
    }
    if (token.type == PARENTHESES && isTokenChar(token, ')')) {
      printNodes(headNode);
      if (currNum.type != UNKNOWN) {
        PRINTF("last ) add %s\n", currNum.string->value);
        // printNode(tailNode, "add last to tailNode");
        leftNode = CNode();
        leftNode->value = currNum;
        addChildNode(tailNode, leftNode, RIGHT);
        currNum = (Token){UNKNOWN, NULL};
      }
      PRINTF(")stack size: %d\n", stack->size);
      State state = pop(stack);
      printf("pop success\n");
      if (state.head == NULL || state.tail == NULL) {
        PRINTF("continue because all null\n");
        continue;
      }
      state.tail->children[RIGHT] = headNode;
      headNode = state.head;
      tailNode = state.tail;
      continue;
    }
    if (token.type == INTEGER || token.type == FLOAT) {
      PRINTF("int %s\n", token.string->value);
      currNum = token;
      continue;
    }
    Node *currNode = CNode();
    currNode->value = token;
    if (currNum.type != UNKNOWN) {
      leftNode = CNode();
      leftNode->value = currNum;
    } else {
      leftNode = headNode;
    }
    addChildNode(currNode, leftNode, LEFT);
    if (tailNode == NULL) {
      PRINTF("current node empty\n");
      headNode = currNode;
      tailNode = currNode;
      continue;
    }
    addExprToTree(currNode, &headNode, &tailNode);
    PRINTF("added to exp tree\n");
    PRINTF("-------------------------------------\n");
  }
  if (currNum.type != UNKNOWN) {
    PRINTF("add last num to tree (%s)\n", tailNode->value.string->value);
    // printNode(tailNode, "add last to tailNode");
    leftNode = CNode();
    leftNode->value = currNum;
    addChildNode(tailNode, leftNode, RIGHT);
    PRINTF("after added last \n");
  }
  PRINTF("calc\n");
  // freeStr(&currNum);
  return headNode;
Fail:
  PRINTF("ENTERED INCORRECT SYMBOLS\n");
  return NULL;
}
State CState(Node *head, Node *tail) {
  State state;
  state.head = head;
  state.tail = tail;
  return state;
}
NodeList *nodeList_new() {
  NodeList *nodeListPtr = (NodeList *)malloc(sizeof(NodeList));
  if (nodeListPtr == NULL) {
    printf("null1\n");
    return (NodeList *)NULL;
  }
  Node **nodePtr = (Node **)malloc(sizeof(Node *) * 10);
  if (nodePtr == NULL) {
    printf("null2\n");
    return (NodeList *)NULL;
  }
  NodeList *nodeList = nodeListPtr;
  nodeList->node = (Node **)nodePtr;
  nodeList->capacity = 10;
  nodeList->size = 0;
  return nodeList;
}
void nodeList_append(NodeList *nodeList, Node *node) {
  printf("no%d, %d\n", nodeList->size + 1, nodeList->capacity);
  printf("wo\n");
  if (nodeList->size + 1 > nodeList->capacity) {
    printf("lol]n\n");
    int capacity = roundCapacity(nodeList->size + 1);
    printf("cap>%d:%d\n", capacity, sizeof(Node *));
    printf("%lu\n", (long unsigned int)nodeList->node);
    Node **ptr = (Node **)realloc(nodeList->node, capacity * sizeof(Node *));
    printf("printf\n");
    printf("%lu\n", (long unsigned int)ptr);
    if (ptr == NULL) {
      printf("null\n");
      return;
    }
    nodeList->node = ptr;
    printf("cappp\n");
    nodeList->capacity = capacity;
  }
  printf("yes\n");
  nodeList->node[nodeList->size] = node;
  nodeList->size++;
}
void printNodes(Node *node) {
  if (node == NULL)
    return;
  if (node->children[LEFT] == NULL || node->children[RIGHT] == NULL)
    return;
  printf("(%s %c %s)", node->children[LEFT]->value.string->value,
         getOperator(*node), node->children[RIGHT]->value.string->value);
  printNodes(node->children[LEFT]);
  printNodes(node->children[RIGHT]);
}
