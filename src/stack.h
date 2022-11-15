#ifndef STACK_H
#define STACK_H

#define MAX_STACK_SIZE 50

#include "expression.h"

void stack_init();

int stack_push(expression_symbols input);

expression_symbols stack_pop();

expression_symbols stack_peek_1();

expression_symbols stack_peek_2();

expression_symbols stack_peek_3();

expression_symbols stack_top_terminal();

int stack_is_full();

int stack_is_empty();

#endif
