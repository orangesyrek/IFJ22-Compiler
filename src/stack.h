#ifndef STACK_H
#define STACK_H

#define MAX_STACK_SIZE 50

#include "expression.h"

void stack_init();

int stack_push(expression_symbols input);

int stack_push_after_top_terminal (expression_symbols input);

void stack_pop();

void stack_pop_times(int times);

expression_symbols stack_peek_1();

expression_symbols stack_peek_2();

expression_symbols stack_peek_3();

expression_symbols stack_get();

expression_symbols stack_top_terminal();

int stack_top_terminal_pos();

int stack_until_shift();

int stack_is_full();

int stack_is_empty();

void stack_print();

#endif
