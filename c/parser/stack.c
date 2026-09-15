#include "headers/stack.h"
#include "headers/useless_util.h"
#include <stdlib.h>

Stack *CStack() {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack->state = (State *)malloc(sizeof(State) * 10);
  stack->size = 0;
  stack->capacity = 10;
  return stack;
}
void push(Stack *stack, State *state) {
  if (stack->size + 1 < stack->capacity) {
    int capacity = roundCapacity(stack->size + 1);
    Stack *ptr = (Stack *)realloc(stack->state, sizeof(State) * capacity);
    if (ptr == NULL) {
      return;
    }
    stack->capacity = capacity;
  }
  memcpy(stack->state + stack->size, state, sizeof(State));
  stack->size++;
}
State pop(Stack *stack) {
  if (stack->size < 1) {
    State state;
    state.head = NULL;
    state.tail = NULL;
    return state;
  }
  return stack->state[--stack->size];
}
void freeStack(Stack *stack) {
  free(stack->state);
  free(stack);
}
