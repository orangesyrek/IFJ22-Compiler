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
	ERR_PRINT("Syntax error in prolog.");
	return COMP_ERR_SA;
}

static comp_err
rule_next_param()
{
	struct lexeme current_token;

	current_token = getToken();
	if (current_token.type == R_PAR) {
		/* next_param -> empty */
		return COMP_OK;
	} else if (current_token.type == COMMA) {
		/* next_param -> , Terminal next_param */
		current_token = getToken();
		if ((current_token.type == STR_LIT) || (current_token.type == INT_LIT) || (current_token.type == DECIMAL_LIT)
		|| (current_token.type == VAR)) {
			return rule_next_param();
		} else {
			return COMP_ERR_SA;
		}
	} else {
		return COMP_ERR_SA;
	}
}

static comp_err
rule_func_params()
{
	int ret = COMP_OK;
	struct lexeme current_token;

	current_token = getToken();
	if (current_token.type == R_PAR) {
		/* func_params -> empty */
		goto cleanup;
	} else if ((current_token.type == STR_LIT) || (current_token.type == INT_LIT) || (current_token.type == DECIMAL_LIT)
		|| (current_token.type == VAR)) {
		/* func_params -> Terminal next_param */
		ret = rule_next_param();
	} else if ((current_token.type == MINUS) || (current_token.type == PLUS)) {
		current_token = getToken();
		if ((current_token.type == INT_LIT) || (current_token.type == DECIMAL_LIT)) {
			ret = rule_next_param();
		} else {
			ERR_PRINT("Syntax error in parsing function call.");
			ret = COMP_ERR_SA;
			goto cleanup;
		}
	} else {
		ERR_PRINT("Syntax error in parsing function call.");
		ret = COMP_ERR_SA;
		goto cleanup;
	}

cleanup:
	return ret;
}

static comp_err
rule_func()
{
	int ret;
	struct lexeme current_token;

	current_token = getToken();
	if (current_token.type != L_PAR) {
		goto cleanup;
	}

	ret = rule_func_params();
	if (ret != COMP_OK) {
		goto cleanup;
	}

	/* if all went well, ) should already be consumed */

	return COMP_OK;

cleanup:
	ERR_PRINT("Syntax error in function call");
	return COMP_ERR_SA;
}


comp_err
synt_parse()
{
	struct lexeme current_token;
	int ret;

	/* rule: program -> prolog statement_list EOF */

	current_token = getToken();
	ret = parse_prolog(current_token);
	if (ret) {
		goto cleanup;
	}

	/* parse lexemes that can appear in the global scope */
	while ((current_token = getToken()).type != LEX_EOF) {
		if (current_token.type == FUN_ID) {
			rule_func();
			current_token = getToken();
			if (current_token.type != SEMICOLON) {
				ret = COMP_ERR_SA;
				goto cleanup;
			}
		} else if (current_token.type == VAR) {

		} else if (current_token.type == KEYWORD_IF) {

		} else if (current_token.type == KEYWORD_WHILE) {

		} else if (current_token.type == KEYWORD_FUNCTION) {

		} else if (current_token.type == L_PAR) {

		} else if (current_token.type == SEMICOLON) {

		} else if (current_token.type == COMMENT) {
			continue;
		} else if (current_token.type == PROLOG_END) {

		}
	}

cleanup:
	return ret;
}
