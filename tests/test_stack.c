#include <stdio.h>

#include "../src/compiler.h"
#include "../src/lexical_analysis.h"
#include "../src/stack.h"
#include "../src/expression.h"
#include "../src/syntactic_analysis.h"

struct compiler_ctx *ctx;

int
main(void)
{
	stack_init();

	stack_push(E_DOLLAR);
	stack_print();

	stack_push(S);
	stack_push(E_ID);
	stack_print();

	int count;
	count = stack_until_shift();
	printf("Count to shift: %d\n", count);

	stack_pop();
	stack_pop();
	stack_push(E_NON_TERM);
	stack_print();

	stack_push_after_top_terminal(S);
	stack_print();

	stack_push(E_PLUS);
	stack_print();

	stack_push(S);
	stack_push(E_ID);
	stack_print();

	stack_pop();
	stack_pop();
	stack_push(E_NON_TERM);
	stack_print();

	expression_symbols terminal;
	terminal = stack_top_terminal();
	if (terminal == E_PLUS) printf("Top terminal: +\n");
	else printf("Top terminal error\n");

	stack_push_after_top_terminal(S);
	stack_print();

	stack_push(E_MUL);
	stack_print();

	stack_push(S);
	stack_push(E_ID);
	stack_print();

	terminal = stack_top_terminal();
	if (terminal == E_ID) printf("Top terminal: i\n");
	else printf("Top terminal error\n");

	stack_pop();
	stack_pop();
	stack_push(E_NON_TERM);
	stack_print();

	count = stack_until_shift();
	printf("Count to shift: %d\n", count);

	terminal = stack_top_terminal();
	if (terminal == E_MUL) printf("Top terminal: *\n");
	else printf("Top terminal error\n");

	stack_pop_times(4);
	stack_push(E_NON_TERM);
	stack_print();

	stack_pop_times(4);
	stack_push(E_NON_TERM);
	stack_print();

	terminal = stack_top_terminal();
	if (terminal == E_DOLLAR) printf("Top terminal: $\n");
	else printf("Top terminal error\n");
}