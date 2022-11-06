#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "syntactic_analysis.h"
#include "lexical_analysis.h"

extern struct compiler_ctx *ctx;

static comp_err rule_statement_list();

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
		|| (current_token.type == VAR) || (current_token.type == KEYWORD_NULL)) {
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
		|| (current_token.type == VAR) || (current_token.type == KEYWORD_NULL)) {
		/* func_params -> Terminal next_param */
		ret = rule_next_param();
	} else if ((current_token.type == MINUS) || (current_token.type == PLUS)) {
		current_token = getToken();
		if ((current_token.type == INT_LIT) || (current_token.type == DECIMAL_LIT)) {
			ret = rule_next_param();
			if (ret != COMP_OK) {
				ERR_PRINT("Syntax error in parsing function call.");
			}
			goto cleanup;
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
	ERR_PRINT("Syntax error in function call.");
	return COMP_ERR_SA;
}

static comp_err
rule_func_def_next_param()
{
	struct lexeme current_token;

	current_token = getToken();
	if (current_token.type == R_PAR) {
		return COMP_OK;
	} else if (current_token.type == COMMA) {
		current_token = getToken();
		if (current_token.type == PARAM_TYPE) {
			current_token = getToken();
			if (current_token.type == VAR) {
				return rule_func_def_next_param();
			} else {
				return COMP_ERR_SA;
			}
		} else {
			return COMP_ERR_SA;
		}
	} else {
		return COMP_ERR_SA;
	}
}

static comp_err
rule_func_def_parameters()
{
	int ret = COMP_OK;
	struct lexeme current_token;

	current_token = getToken();
	if (current_token.type == R_PAR) {
		goto cleanup;
	} else if (current_token.type == PARAM_TYPE) {
		current_token = getToken();
		if (current_token.type == VAR) {
			ret = rule_func_def_next_param();
			if (ret != COMP_OK) {
				ERR_PRINT("Syntax error in function definiton parameters.");
			}
			goto cleanup;
		} else {
			ret = COMP_ERR_SA;
			ERR_PRINT("Syntax error in function definiton parameters.");
			goto cleanup;
		}
	} else {
		ret = COMP_ERR_SA;
		ERR_PRINT("Syntax error in function definiton parameters.");
		goto cleanup;
	}

cleanup:
	return ret;
}

/* statement -> function function_name_T ( parameters ) : type_T { statement_list } */
static comp_err
rule_func_def()
{
	int ret = COMP_OK;
	struct lexeme current_token;

	ctx->in_function = 1;

	current_token = getToken();
	if (current_token.type != FUN_ID) {
		goto cleanup;
	}

	current_token = getToken();
	if (current_token.type != L_PAR) {
		goto cleanup;
	}

	ret = rule_func_def_parameters();
	if (ret != COMP_OK) {
		goto cleanup;
	}

	/* if all went well, ) should already be consumed */

	current_token = getToken();
	if (current_token.type != COLON) {
		goto cleanup;
	}

	current_token = getToken();
	if ((current_token.type != PARAM_TYPE) && (current_token.type != KEYWORD_VOID)) {
		goto cleanup;
	}

	current_token = getToken();
	if (current_token.type != L_CURLY) {
		goto cleanup;
	}

	ret = rule_statement_list();
	if (ret != COMP_OK) {
		goto cleanup;
	}

	/* if all went well, } should already be consumed */
	ctx->in_function = 0;
	return COMP_OK;

cleanup:
	ERR_PRINT("Syntax error in function definition.");
	return COMP_ERR_SA;
}

static comp_err
rule_if_statement()
{
	int ret = COMP_OK;
	struct lexeme current_token;

	current_token = getToken();
	if (current_token.type != L_PAR) {
		goto cleanup;
	}

	/* call expr */
	while ((current_token = getToken()).type != R_PAR);

	current_token = getToken();
	if (current_token.type != L_CURLY) {
		goto cleanup;
	}

	ret = rule_statement_list();
	if (ret != COMP_OK) {
		goto cleanup;
	}

	if (ctx->last_token != R_CURLY) {
		goto cleanup;
	}

	current_token = getToken();
	if (current_token.type != KEYWORD_ELSE) {
		goto cleanup;
	}

	current_token = getToken();
	if (current_token.type != L_CURLY) {
		goto cleanup;
	}

	ret = rule_statement_list();
	if (ret != COMP_OK) {
		goto cleanup;
	}

	if (ctx->last_token != R_CURLY) {
		goto cleanup;
	}

	return COMP_OK;
cleanup:
	ERR_PRINT("Syntax error in if statement.");
	return COMP_ERR_SA;
}

static comp_err
rule_var_declaration()
{
	struct lexeme current_token;

	current_token = getToken();
	if (current_token.type == ASSIGNMENT) {
		// expr
		while ((current_token = getToken()).type != SEMICOLON);
		return COMP_OK;
	} else if (current_token.type == SEMICOLON) {
		return COMP_OK;
	} else {
		goto cleanup;
	}

cleanup:
	ERR_PRINT("Syntax error in variable declaration");
	return COMP_ERR_SA;
}

static comp_err
rule_while_statement()
{
	int ret = COMP_OK;
	struct lexeme current_token;

	current_token = getToken();
	if (current_token.type != L_PAR) {
		goto cleanup;
	}

	// expr
	while ((current_token = getToken()).type != R_PAR);

	current_token = getToken();
	if (current_token.type != L_CURLY) {
		goto cleanup;
	}

	ret = rule_statement_list();
	if (ret != COMP_OK) {
		goto cleanup;
	}

	if (ctx->last_token != R_CURLY) {
		goto cleanup;
	}

	/* ungetc('}', stdin);
	current_token = getToken();
	if (current_token.type != R_CURLY) {
		goto cleanup;
	} */

	return COMP_OK;

cleanup:
	ERR_PRINT("Syntax error in while loop");
	return COMP_ERR_SA;
}


/* rule: program -> prolog statement_list EOF */
static comp_err
rule_statement_list()
{
	int ret;
	struct lexeme current_token;

	/* parse lexemes that can appear in the global scope */
	ctx->last_token = -1;
	current_token = getToken();
	if (current_token.type == FUN_ID) {
		ret = rule_func();
		if (ret != COMP_OK) {
			goto cleanup;
		}
		current_token = getToken();
		if (current_token.type != SEMICOLON) {
			ret = COMP_ERR_SA;
			goto cleanup;
		}
	} else if (current_token.type == VAR) {
		ret = rule_var_declaration();
		if (ret != COMP_OK) {
			goto cleanup;
		}
	} else if (current_token.type == KEYWORD_IF) {
		ret = rule_if_statement();
		if (ret != COMP_OK) {
			goto cleanup;
		}
	} else if (current_token.type == KEYWORD_WHILE) {
		ret = rule_while_statement();
		if (ret != COMP_OK) {
			goto cleanup;
		}
	} else if (current_token.type == KEYWORD_FUNCTION) {
		if (ctx->in_function) {
			ret = COMP_ERR_SA;
			goto cleanup;
		}

		ret = rule_func_def();
		if (ret != COMP_OK) {
			goto cleanup;
		}
	} else if (current_token.type == L_PAR) {

	} else if (current_token.type == SEMICOLON) {
		return COMP_OK;
	} else if (current_token.type == COMMENT) {
		return rule_statement_list();
	} else if (current_token.type == PROLOG_END) {

	} else if (current_token.type == LEX_EOF) {
		return COMP_OK;
	} else if (current_token.type == KEYWORD_RETURN) {

	} else if (current_token.type == R_CURLY) {
		ctx->last_token = R_CURLY;
		return COMP_OK;
	} else {
		return COMP_ERR_SA;
	}

	return rule_statement_list();

cleanup:
	ERR_PRINT("Syntax parsing failed");
	return ret;
}


comp_err
synt_parse()
{
	struct lexeme current_token;
	int ret;

	current_token = getToken();
	ret = parse_prolog(current_token);
	if (ret) {
		goto cleanup;
	}

	ret = rule_statement_list();
	if (ret) {
		goto cleanup;
	}

cleanup:
	return ret;
}
