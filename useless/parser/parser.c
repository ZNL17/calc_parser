#include "headers/parser.h"
#include "headers/expression.h"
#include "headers/lexer.h"
#include "headers/new_string.h"
#include "headers/useless_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#define PRINTF
int main(int argc, char *argv[]) {
  if (argc < 2) {
    return EXIT_FAILURE;
  }
  if (argc == 2) {
    parse(argv);
  }
  if (!strcmp(argv[1], "-f")) {
    fileParse(argv);
  }
}
void parse(char *argv[]) {
  Tokens *tokens = lexer(argv[1]);
  printf("lex:----------------\n");
  printTokens(tokens);
  negation_rule(tokens);
  printf("neg:-___---------------\n");
  printTokens(tokens);
  int i = 0;
  printf("lol\n");
  Node *headNode = recursivParse(tokens, &i, 0);
  if (headNode == NULL) {
    return;
  }
  Number result = calc(headNode);
  printNumber(result);
}
void fileParse(char *argv[]) {

  FILE *file = fopen(argv[2], "r");
  if (file == NULL) {
    return;
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
      return;
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
int negation_rule(Tokens *tokens) {
  Tokens *new_tokens = tokens_new();
  int sign = 0;
  for (int i = 0; i < tokens->size; i++) {
    Token *token = tokens->tokens + i;
    if (isTokenChar(*token, '-')) {
      if (sign == 0) {
        sign = 1;
        if (!i || !isTokenNumber((tokens->tokens + i - 1)->type)) {
          continue;
        }
        goto Next;
      }
      sign *= -1;
      continue;
    }
    if (sign != 0 && (token->type == INTEGER || token->type == FLOAT)) {
      char negNum[token->string->size + 1];
      negNum[0] = '-';
      strcat(negNum, token->string->value);
      printf("strcat: %s\n", negNum);
      string_set_cstrings(token->string, negNum);
      printf("append: %s\n", token->string->value);
    }
    sign = 0;
  Next:
    appendToken(new_tokens, token);
    printf("%d: \n", i);
    printTokens(new_tokens);
  }
  tokens->tokens = new_tokens->tokens;
  tokens->size = new_tokens->size;
  return 1;
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
    int capacity = roundCapacity(nodeList->size + 1);
    printf("%lu\n", (long unsigned int)nodeList->node);
    Node **ptr = (Node **)realloc(nodeList->node, capacity * sizeof(Node *));
    printf("%lu\n", (long unsigned int)ptr);
    if (ptr == NULL) {
      return;
    }
    nodeList->node = ptr;
    nodeList->capacity = capacity;
  }
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
