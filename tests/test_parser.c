#include <stdio.h>

#include "../src/compiler.h"
#include "../src/lexical_analysis.h"
#include "../src/syntactic_analysis.h"
#include "../src/symtab.h"

struct compiler_ctx *ctx = NULL;

int
main(void)
{
	int ret;

	if (compiler_ctx_new(&ctx)) {
		return 1;
	}

	ret = insert_builtin_functions(&ctx->global_sym_tab);
	if (ret) {
		return COMP_ERR_INTERNAL;
	}

	ret = synt_parse();
	if (ret != COMP_OK) {
		return ret;
	}

	printf("test ok \n");
	return 0;
}
