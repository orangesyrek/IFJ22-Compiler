#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "lexical_analysis.h"
#include "syntactic_analysis.h"

#define DEBUGLEX

struct compiler_ctx *ctx = NULL;

int
main(int argc, char **argv)
{
	int ret = COMP_OK;

	(void) argc;
	(void) argv;

	ret = compiler_ctx_new(&ctx);
	if (ret) {
		return COMP_ERR_INTERNAL;
	}

	ret = synt_parse();
	if (ret) {
		return ret;
	}

	compiler_ctx_destroy(ctx);
	return 0;
}
