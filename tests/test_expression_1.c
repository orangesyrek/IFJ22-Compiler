#include <stdio.h>

#include "../src/stack.h"
#include "../src/expression.h"

struct compiler_ctx *ctx;

int
main(void)
{
	expression_symbols a;
	expression_symbols b;
	expression_symbols res;

	a = E_PLUS;
	b = E_MUL;
	res = get_op(a, b);

	printf("a: +\n");
	printf("b: *\n");
	printf("op: S\n");

	if (res == S) printf("result ok\n");
	else printf("result error\n");

	a = E_L_BRACKET;
	b = E_R_BRACKET;
	res = get_op(a, b);

	printf("a: (\n");
	printf("b: )\n");
	printf("op: E\n");

	if (res == E) printf("result ok\n");
	else printf("result error\n");

	a = E_R_BRACKET;
	b = E_L_BRACKET;
	res = get_op(a, b);

	printf("a: )\n");
	printf("b: (\n");
	printf("op: S\n");

	if (res == X) printf("result ok\n");
	else printf("result error\n");
}