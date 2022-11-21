#include <stdlib.h>
#include <stdio.h>

#include "stack.h"

int top;
stack_item stack[MAX_STACK_SIZE];

void
stack_init()
{
  top = -1;
}

int
stack_push(expression_symbols symbol, lex_types type, struct lexeme token)
{

  if (stack_is_full()) {
    return 1;
  }

  stack_item new_item;
  new_item.symbol = symbol;
  new_item.type = type;
  new_item.token = token;

  top += 1;
  stack[top] = new_item;
  return 0;

  printf(" %d", top);
}

int
stack_push_after_top_terminal(expression_symbols symbol, lex_types type, struct lexeme token)
{
  //printf("stack_push_after_top_terminal() start\n");
  if (stack_is_full())
  {
    return 1;
  }

  stack_item new_item;
  new_item.symbol = symbol;
  new_item.type = type;
  new_item.token = token;

  int pos = stack_top_terminal_pos() + 1;
  int insert_pos = pos;
  top += 1;

  //printf("pos: %d\n", pos);
  //printf("insert_pos: %d\n", pos);
  //printf("top: %d\n", top);

  while (pos != top)
  {
    //printf("while start\n");

    stack[pos+1] = stack[pos];
    pos++;

    //printf("pos: %d\n", pos);
    //printf("while end\n");
  }

  //printf("insert_pos: %d\n", pos);
  stack[insert_pos] = new_item;
  //printf("stack_push_after_top_terminal() end\n");
  return 0;
}

void
stack_pop()
{
  if (!stack_is_empty()) {
    top -= 1; 
  }
}

void
stack_pop_times(int times)
{
  for (int i = 0; i < times; i++)
  {
    stack_pop();
  }
}

stack_item
stack_peek_1()
{
  return stack[top];
}

stack_item
stack_peek_2()
{
  return stack[top-1];
}

stack_item
stack_peek_3()
{
  return stack[top-2];
}

stack_item
stack_get(int pos)
{
  return stack[pos];
}

stack_item
stack_top_terminal()
{

  for (int i = top; i != -1; i--) {

    if(stack_get(i).symbol != E_NON_TERM && stack_get(i).symbol != R && stack_get(i).symbol != S && stack_get(i).symbol != E && stack_get(i).symbol != X)
    {
      stack_item terminal = stack_get(i);
      return terminal;
    }
  }
  stack_item terminal;
  terminal.symbol = X;
  terminal.type = 0;
  return terminal;
}

int
stack_top_terminal_pos()
{

  for (int i = top; i != -1; i--) {

    if(stack_get(i).symbol != E_NON_TERM && stack_get(i).symbol != R && stack_get(i).symbol != S && stack_get(i).symbol != E && stack_get(i).symbol != X)
    {
      return i;
    }
  }
  return -1;
}

int
stack_until_shift()
{
  int count = 0;

  for (int i = top; i != -1; i--)
  {
    if (stack_get(i).symbol == S)
    {
      return count;
    }
    count++;
  }
  return -1;
}

int
stack_is_full()
{
  if (top == (MAX_STACK_SIZE - 1)) {
    return 1;
  }
  return 0;
}

int
stack_is_empty()
{
  if (top == -1) {
    return 1;
  }
  return 0;
}

void stack_print()
{
  int pos = -1;
  printf("Stack: ");
  while (pos != top)
  {
    pos += 1;
    switch(stack_get(pos).symbol)
    {
      case E_PLUS:
        printf("+");
        break;
      case E_MINUS:
        printf("-");
        break;
      case E_CON:
        printf(".");
        break;
      case E_MUL:
        printf("*");
        break;
      case E_DIV:
        printf("/");
        break;
      case E_LT:
        printf("<");
        break;
      case E_LEQ:
        printf("<=");
        break;
      case E_GT:
        printf(">");
        break;
      case E_GEQ:
        printf(">=");
        break;
      case E_EQ:
        printf("===");
        break;
      case E_NEQ:
        printf("!==");
        break;
      case E_L_BRACKET:
        printf("(");
        break;
      case E_R_BRACKET:
        printf(")");
        break;
      case E_ID:
        printf("i");
        break;
      case E_DOLLAR:
        printf("$");
        break;
      case E_NON_TERM:
        printf("E");
        break;
      case R:
        printf(">");
        break;
      case S:
        printf("<");
        break;
      case E:
        printf("=");
        break;
      default:
        printf("ERROR");
        break;
    }
  }
  printf("\n");
}