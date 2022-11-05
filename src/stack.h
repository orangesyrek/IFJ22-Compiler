#ifndef STACKH
#define STACKH

#define MAX_STACK_SIZE 50

void stack_init();

int push(expression_symbols input);

expression_symbols pop();

expression_symbols peek();

int is_full();

int is_empty();

#endif
