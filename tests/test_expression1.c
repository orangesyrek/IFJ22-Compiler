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
	expression_symbols a;
	expression_sybmols b;
	expression_symbols res;

	a = E_PLUS;
	b = E_MUL;
	res = get_op(a, b);

	printf("a: +\n");
	printf("b: *\n");
	printf("op: S\n");

	if (res == S) printf("result ok\n");
	else printf("result error\n");

}