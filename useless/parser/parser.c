#include "../headers/new_string.h"
#include "useless_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ADD 43
#define SUBSTRACT 45
#define MULTPLY 42
#define DIVIDE 47
typedef enum {
  Add = ADD,
  Substract = SUBSTRACT,
  Multply = MULTPLY,
  Divide = DIVIDE
} Operator;
typedef struct {
  int leftValue;
  int rightValue;
  Operator op;
} Operation;
typedef struct {
  Operation *list;
  int size;
  int capacity;
  int growth;
} List;
int calc_p(Operation *operation);
int calc_cpy(Operation operation);
Operation createOp(const int a, const int b, const char *operator);
void setOperator(Operation *op, const char *operator);
List createList();
void append(List *list, Operation op);
int main(int argc, char *argv[]) {
  if (argc != 2)
    return EXIT_FAILURE;
  List list = createList();
  char *content = argv[1];
  String currNum = createStr("");
  while (*content) {
    if (isNumber(*content)) {
      appendCStr(&currNum, content);
    }
    content++;
  }
  return EXIT_SUCCESS;
}

int calc_p(Operation *op) {
  switch (op->op) {
  case Add:
    return op->leftValue + op->rightValue;
    break;
  case Substract:
    return op->leftValue - op->rightValue;
    break;
  case Multply:
    return op->leftValue * op->rightValue;
    break;
  case Divide:
    return op->leftValue / op->rightValue;
    break;
  }
}
int calc_cpy(Operation op) {
  switch (op.op) {
  case Add:
    return op.leftValue + op.rightValue;
    break;
  case Substract:
    return op.leftValue - op.rightValue;
    break;
  case Multply:
    return op.leftValue * op.rightValue;
    break;
  case Divide:
    return op.leftValue / op.rightValue;
    break;
  }
}
Operation createOp(const int a, const int b, const char *operator) {
  Operation op;
  op.leftValue = a;
  op.rightValue = b;
  return op;
}
void setOperator(Operation *op, const char *operator) {
  switch (*operator) {
  case Add:
    op->op = Add;
    break;
  case Substract:
    op->op = Substract;
    break;
  case Multply:
    op->op = Multply;
    break;
  case Divide:
    op->op = Divide;
    break;
  }
}
List createList() {
  List list;
  list.list = NULL;
  list.capacity = 0;
  list.size = 0;
  list.growth = 10;
  return list;
}
void append(List *list, Operation op) {
  if (list->capacity + 1 > list->size) {
    Operation *ptr =
        realloc(list->list, sizeof(Operation) * list->size + list->growth);
    if (ptr == NULL)
      return;
    list->list = ptr;
    list->size += list->growth;
  }
  list->list[++list->capacity] = op;
}
void freeList(List *list) { free(list->list); }
