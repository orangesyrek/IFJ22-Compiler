#ifndef STACK_H
#define STACK_H

#define MAX_STACK_SIZE 50

#include "expression.h"

void stack_init();

int stack_push(expression_symbols symbol, lex_types type, struct lexeme token);

int stack_push_after_top_terminal (expression_symbols symbol, lex_types type, struct lexeme token);

void stack_pop();

void stack_pop_times(int times);

stack_item stack_peek_1();

stack_item stack_peek_2();

stack_item stack_peek_3();

stack_item stack_get();

stack_item stack_top_terminal();

int stack_top_terminal_pos();

int stack_until_shift();

int stack_is_full();

int stack_is_empty();

void stack_print();

#endif
