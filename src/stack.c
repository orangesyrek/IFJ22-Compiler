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
stack_push(expression_symbols input)
{
  if (stack_is_full()) {
    return 1;
  }

  top += 1;
  stack[top] = input;
  return 0;
}

void
stack_stack_pop()
{
  if (!stack_is_empty()) {
    top -= 1; 
  }
}

expression_symbols
stack_peek_1()
{
  return stack[top];
}

expression_symbols
stack_peek_2()
{
  return stack[top-1];
}

expression_symbols
stack_peek_3()
{
  return stack[top-2];
}

expression_symbols
stack_top_terminal()
{
  int prev_top = top;

  for (int i = top; top != -1; i--) {

    if(stack_peek_1() != E_NON_TERM || stack_peek_1() != R || stack_peek_1() != S || stack_peek_1() != E || stack_peek_1() != X)
    {
      expression_symbols terminal = stack_peek_1();
      top = prev_top;
      return terminal;
    }
  }
}

int
stack_is_full()
{
  if (top == (MAX_STACK_SIZE - 1)) {
    return 0;
  }
  return 1;
}

int
stack_is_empty()
{
  if (top == -1) {
    return 0;
  }
  return 1;
}
