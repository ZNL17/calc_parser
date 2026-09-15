#ifndef STACK_H
#define STACK_H
#include "parser.h"
typedef struct {
  int size;
  int capacity;
  State *state;
} Stack;

Stack *CStack();
void push(Stack *stack, State *state);
State pop(Stack *stack);
void freeStack(Stack *stack);
#endif
