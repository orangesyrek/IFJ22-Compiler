#ifndef STACK_H
#define STACK_H

#define MAX_STACK_SIZE 50

#include "expression.h"

void stack_init();

int push(expression_symbols input);

expression_symbols pop();

expression_symbols peek();

int is_full();

int is_empty();

#endif
