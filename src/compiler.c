/*
 * FIT VUT 2022 - IFJ Project
 * Implementation of a compiler for an imperative language IFJ22
 *
 * File: compiler.c
 * Author(s): xjanot04
 */

#include <stdlib.h>

#include "compiler.h"
#include "symtab.h"

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
	symtabDispose(&ctx->global_sym_tab);
	symtabDispose(&ctx->local_sym_tab);
	free(ctx);
}
