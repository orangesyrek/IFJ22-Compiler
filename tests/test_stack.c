#include <stdio.h>

#include "../src/compiler.h"
#include "../src/lexical_analysis.h"
#include "../src/stack.h"
#include "../src/expression.h"
#include "../src/syntactic_analysis.h"

int
main(void)
{
	stack_init();

	stack_push(E_DOLLAR);
	stack_print();
	printf("\n");

	stack_push(E_ID);
	stack_print();
	printf("\n");

	stack_push(E_PLUS);
	stack_print();
	printf("\n");

	stack_push(E_ID);
	stack_print();
	printf("\n");
}