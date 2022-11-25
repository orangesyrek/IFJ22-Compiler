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
	struct lexeme token = {0};
	stack_init();

	stack_push(E_DOLLAR, 0, token);
	stack_print();

	stack_push(S, 0, token);
	stack_push(E_ID, 0, token);
	stack_print();

	int count;
	count = stack_until_shift();
	printf("Count to shift: %d\n", count);

	stack_pop();
	stack_pop();
	stack_push(E_NON_TERM, 0, token);
	stack_print();

	stack_push_after_top_terminal(S, 0, token);
	stack_print();

	stack_push(E_PLUS, 0, token);
	stack_print();

	stack_push(S, 0, token);
	stack_push(E_ID, 0, token);
	stack_print();

	stack_pop();
	stack_pop();
	stack_push(E_NON_TERM, 0, token);
	stack_print();

	expression_symbols terminal;
	terminal = stack_top_terminal().symbol;
	if (terminal == E_PLUS) printf("Top terminal: +\n");
	else printf("Top terminal error\n");

	stack_push_after_top_terminal(S, 0, token);
	stack_print();

	stack_push(E_MUL, 0, token);
	stack_print();

	stack_push(S, 0, token);
	stack_push(E_ID, 0, token);
	stack_print();

	terminal = stack_top_terminal().symbol;
	if (terminal == E_ID) printf("Top terminal: i\n");
	else printf("Top terminal error\n");

	stack_pop();
	stack_pop();
	stack_push(E_NON_TERM, 0, token);
	stack_print();

	count = stack_until_shift();
	printf("Count to shift: %d\n", count);

	terminal = stack_top_terminal().symbol;
	if (terminal == E_MUL) printf("Top terminal: *\n");
	else printf("Top terminal error\n");

	stack_pop_times(4);
	stack_push(E_NON_TERM, 0, token);
	stack_print();

	stack_pop_times(4);
	stack_push(E_NON_TERM, 0, token);
	stack_print();

	terminal = stack_top_terminal().symbol;
	if (terminal == E_DOLLAR) printf("Top terminal: $\n");
	else printf("Top terminal error\n");
}