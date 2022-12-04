/*
 * FIT VUT 2022 - IFJ Project
 * Implementation of a compiler for an imperative language IFJ22
 *
 * File: main.c
 * Author(s): xbarta50, xjanot04, xpauli08, xvalik04
 */

#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "generator.h"
#include "lexical_analysis.h"
#include "symtab.h"
#include "syntactic_analysis.h"

struct compiler_ctx *ctx = NULL;

int
main(void)
{
	int ret = COMP_OK;

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

	return ret;
}
