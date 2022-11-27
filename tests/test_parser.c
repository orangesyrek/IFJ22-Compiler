#include <stdio.h>

#include "../src/compiler.h"
#include "../src/lexical_analysis.h"
#include "../src/syntactic_analysis.h"
#include "../src/symtab.h"
#include "../src/generator.h"

struct compiler_ctx *ctx = NULL;

int
main(int argc, char **argv)
{
	int ret = COMP_OK;

	(void) argc;
	(void) argv;

	ret = compiler_ctx_new(&ctx);
	if (ret) {
		printf("new ctx failed\n");
		return COMP_ERR_INTERNAL;
	}

	ret = insert_builtin_functions(&ctx->global_sym_tab);
	if (ret) {
		printf("insert builtin functions failed\n");
		return COMP_ERR_INTERNAL;
	}

	generatorInit();

	ret = synt_parse();
	if (ret) {
		printf("parsing failed\n");
		return ret;
	}

	ret = check_undefined_functions();
	if (ret) {
		printf("check undefined functions failed\n");
		return ret;
	}

	ret = generator_finish();
	if (ret) {
		printf("generator finish failed\n");
		return ret;
	}

	compiler_ctx_destroy(ctx);
	printf("test ok\n");
	return 0;
}
