#include <stdlib.h>
#include "compiler.h"

int
compiler_ctx_new(struct compiler_ctx **ctx)
{
	*ctx = calloc(1, sizeof **ctx);
	if (!*ctx) {
		return 1;
	}
	(*ctx)->current_row = 1;

	return 0;
}

void
compiler_ctx_destroy(struct compiler_ctx *ctx)
{
	free(ctx);
}
