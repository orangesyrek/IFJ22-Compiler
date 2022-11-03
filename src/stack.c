#include <stdlib.h>
#include "stack.h"
#include "expression.h"

extern int top;
extern expression_symbols stack[MAX_STACK_SIZE];

int top;
expression_symbols stack[MAX_STACK_SIZE];

void
stack_init()
{
  top = -1;
}

int
push(expression_symbols input)
{
  if (is_full()) {
    return 1;
  }

  top += 1;
  stack[top] = input;
  return 0;
}

expression_symbols
pop()
{
  char ret;

  if (is_empty()) {
    return 0;
  }

  ret = stack[top];
  top -= 1;
  return ret;
}

expression_symbols
peek()
{
  return stack[top];
}

int
is_full()
{
  if (top == (MAX_STACK_SIZE - 1)) {
    return 0;
  }
  return 1;
}

int
is_empty()
{
  if (top == -1) {
    return 0;
  }
  return 1;
}
