#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "lexical_analysis.h"

struct compiler_ctx *ctx = NULL;

int
main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	if (compiler_ctx_new(&ctx)) {
		return 1;
	}

	print_ctx();
	compiler_ctx_destroy(ctx);
	return 0;
}
