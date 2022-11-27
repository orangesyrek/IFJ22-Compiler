#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "lexical_analysis.h"
#include "syntactic_analysis.h"
#include "symtab.h"
#include "generator.h"

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

	ret = insert_builtin_functions(&ctx->global_sym_tab);
	if (ret) {
		return COMP_ERR_INTERNAL;
	}

	generatorInit();

	ret = synt_parse();
	if (ret) {
		return ret;
	}

	ret = check_undefined_functions();
	if (ret) {
		return ret;
	}


	ret = generator_finish();
	if (ret) {
		return ret;
	}
	generatorWriteCode();

	compiler_ctx_destroy(ctx);
	return 0;
}
