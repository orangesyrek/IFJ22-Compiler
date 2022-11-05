#include <stdio.h>

#include "../src/compiler.h"
#include "../src/lexical_analysis.h"
#include "../src/syntactic_analysis.h"

struct compiler_ctx *ctx = NULL;

int
main(void)
{
	int ret;

	if (compiler_ctx_new(&ctx)) {
		return 1;
	}

	ret = synt_parse();
	if (ret != COMP_OK) {
		return ret;
	}

	return 0;
}
