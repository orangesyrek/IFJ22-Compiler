#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "syntactic_analysis.h"
#include "lexical_analysis.h"

extern struct compiler_ctx *ctx;

static comp_err
parse_prolog(struct lexeme token)
{
	if (token.type != PROLOG_START) {
		goto cleanup;
	}

	token = getToken();
	while (token.type == COMMENT) {
		token = getToken();
	}

	if (token.type != KEYWORD_DECLARE) {
		goto cleanup;
	}

	token = getToken();
	if (token.type != L_PAR) {
		goto cleanup;
	}

	token = getToken();
	if (token.type != KEYWORD_STRICT_TYPES) {
		goto cleanup;
	}

	token = getToken();
	if (token.type != ASSIGNMENT) {
		goto cleanup;
	}

	token = getToken();
	if ((token.type != INT_LIT) || (token.value.int_val != 1)) {
		goto cleanup;
	}

	token = getToken();
	if (token.type != R_PAR) {
		goto cleanup;
	}

	token = getToken();
	if (token.type != SEMICOLON) {
		goto cleanup;
	}

	return COMP_OK;

cleanup:
	ERR_PRINT("Syntax error in prolog");
	return COMP_ERR_SA;
}


comp_err
synt_analyze()
{
	struct lexeme current_token;
	int ret;

	current_token = getToken();
	ret = parse_prolog(current_token);
	if (ret) {
		goto cleanup;
	}

	/* parse lexemes that can appear in the global scope */
	while ((current_token = getToken()) != LEX_EOF) {
		if (current_token == FUN_ID) {

		} else if (current_token == VAR) {

		} else if (current_token == KEYWORD_IF) {

		} else if (current_token == KEYWORD_WHILE) {

		} else if (current_token == KEYWORD_FUNCTION) {

		} else if (current_token == L_PAR) {

		} else if (current_token == SEMICOLON) {

		} else if (current_token == COMMENT) {
			continue;
		} else if (current_token == PROLOG_END) {

		}
	}

cleanup:
	return ret;
}
