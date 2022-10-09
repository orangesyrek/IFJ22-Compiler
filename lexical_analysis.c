#include <stdlib.h>
#include <stdio.h>

#include "compiler.h"

extern struct compiler_ctx *ctx;

void print_ctx()
{
	printf("%d %d\n", ctx->last_read, ctx->current_row);
	ctx->current_row = 1;
}