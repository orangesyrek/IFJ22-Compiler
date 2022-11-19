#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "syntactic_analysis.h"
#include "lexical_analysis.h"
#include "expression.h"
#include "symtab.h"
#include "compiler.h"

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
	while (token.type == COMMENT) {
		token = getToken();
	}

	if (token.type != KEYWORD_STRICT_TYPES) {
		goto cleanup;
	}

	token = getToken();
	if (token.type != ASSIGNMENT) {
		goto cleanup;
	}

	token = getToken();
	while (token.type == COMMENT) {
		token = getToken();
	}

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

static type
token_get_param_type(char *id)
{
	if (!strcmp(id, "int")) {
		return INT;
	} else if (!strcmp(id, "float")) {
		return FLOAT;
	} else if (!strcmp(id, "string")) {
		return STRING;
	} else if (!strcmp(id, "?string")) {
		return QSTRING;
	} else if (!strcmp(id, "?float")) {
		return QFLOAT;
	} else if (!strcmp(id, "?int")) {
		return QINT;
	} else {
		return VOID;
	}
}

static comp_err
rule_func_def_next_param(struct function_data *data)
{
	struct lexeme current_token;

	current_token = getToken();
	if (current_token.type == R_PAR) {
		return COMP_OK;
	} else if (current_token.type == COMMA) {
		current_token = getToken();
		if (current_token.type == PARAM_TYPE) {
			/* next param */
			data->param_count++;
			data->params = realloc_func_params(data->params, data->param_count);
			if (!data->params) {
				return COMP_ERR_INTERNAL;
			}
			data->params[data->param_count - 1] = token_get_param_type(current_token.id);
			current_token = getToken();
			if (current_token.type == VAR) {
				return rule_func_def_next_param(data);
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
rule_func_def_parameters(struct function_data *data)
{
	int ret = COMP_OK;
	struct lexeme current_token;

	current_token = getToken();
	if (current_token.type == R_PAR) {
		/* no params */
		data->param_count = 0;
		goto cleanup;
	} else if (current_token.type == PARAM_TYPE) {
		/* insert new param */
		data->param_count++;
		data->params = realloc_func_params(data->params, data->param_count);
		if (!data->params) {
			ret = COMP_ERR_INTERNAL;
			goto cleanup;
		}
		data->params[0] = token_get_param_type(current_token.id);
		current_token = getToken();
		if (current_token.type == VAR) {
			ret = rule_func_def_next_param(data);
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
	int ret = COMP_ERR_SA;
	int rc;
	struct lexeme current_token;
	struct bs_data *data;
	char *function_name;

	ctx->in_function = 1;

	current_token = getToken();
	if (current_token.type != FUN_ID) {
		goto cleanup;
	}

	/* save function name for later */
	function_name = current_token.id;

	/* check redefinition */
	data = symtabSearch(ctx->global_sym_tab, current_token.id);
	if (data) {
		/* redefiniton found */
		ret = COMP_ERR_UNDEF_FUNC;
		ERR_PRINT("Function redefinition attempted.");
		goto cleanup;
	} else {
		/* init new data */
		rc = dataInit(&data);
		if (rc) {
			ret = rc;
			goto cleanup;
		}
		data->is_function = 1;
		data->data.fdata.is_defined = 1;
	}

	while ((current_token = getToken()).type == COMMENT);
	if (current_token.type != L_PAR) {
		goto cleanup;
	}

	ret = rule_func_def_parameters(&(data->data.fdata));
	if (ret != COMP_OK) {
		goto cleanup;
	}

	/* if all went well, ) should already be consumed */

	while ((current_token = getToken()).type == COMMENT);
	if (current_token.type != COLON) {
		goto cleanup;
	}

	current_token = getToken();
	if ((current_token.type != PARAM_TYPE) && (current_token.type != KEYWORD_VOID)) {
		goto cleanup;
	}
	/* insert return type */
	if (current_token.type == PARAM_TYPE) {
		data->data.fdata.return_type = token_get_param_type(current_token.id);
	} else {
		data->data.fdata.return_type = VOID;
	}

	while ((current_token = getToken()).type == COMMENT);
	if (current_token.type != L_CURLY) {
		goto cleanup;
	}

	/* finally insert the function into the symbol table */
	ret = symtabInsert(&ctx->global_sym_tab, function_name, data);
	if (ret) {
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
	return ret;
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

	if (expression_parse(current_token, current_token) != COMP_OK) {
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
	struct lexeme next_token;
	int ret = COMP_OK;

	current_token = getToken();
	if (current_token.type == ASSIGNMENT) {
		next_token = getToken();
		if (next_token.type == FUN_ID) {
			ret = rule_func();
			if (ret) {
				goto cleanup;
			}

			next_token = getToken();
			if (next_token.type != SEMICOLON) {
				goto cleanup;
			} else {
				return COMP_OK;
			}
		} else {
			if (expression_parse(current_token, next_token)) {
				goto cleanup;
			} else {
				return COMP_OK;
			}
		}
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

	if (expression_parse(current_token, current_token) != COMP_OK) {
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
	ERR_PRINT("Syntax error in while loop");
	return COMP_ERR_SA;
}

static comp_err
rule_prolog_end()
{
	struct lexeme current_token;

	current_token = getToken();
	if (current_token.type != LEX_EOF) {
		goto cleanup;
	}

	return COMP_OK;

cleanup:
	ERR_PRINT("Unexpected token after prolog end");
	return COMP_ERR_SA;
}

static comp_err
rule_return()
{
	struct lexeme current_token = {0};
	struct lexeme next_token;

	next_token = getToken();
	if (next_token.type != SEMICOLON) {
		if (expression_parse(current_token, next_token)) {
			ERR_PRINT("Syntax error in return");
			return COMP_ERR_SA;
		}
	}

	return COMP_OK;
}

/* rule: program -> prolog statement_list EOF */
static comp_err
rule_statement_list()
{
	int ret;
	struct lexeme current_token;
	struct lexeme tmp = {0};

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
		if (expression_parse(current_token, current_token) != COMP_OK) {
			goto cleanup;
		} else {
			return COMP_OK;
		}
	} else if (current_token.type == SEMICOLON) {
		return COMP_OK;
	} else if (current_token.type == COMMENT) {
		return rule_statement_list();
	} else if (current_token.type == PROLOG_END) {
		ret = rule_prolog_end();
		if (ret != COMP_OK) {
			goto cleanup;
		}
	} else if (current_token.type == LEX_EOF) {
		return COMP_OK;
	} else if (current_token.type == KEYWORD_RETURN) {
		if (ctx->in_function) {
			ret = rule_return();
			if (ret != COMP_OK) {
				goto cleanup;
			}
		} else {
			current_token = getToken();
			if (current_token.type == SEMICOLON) {
				return COMP_OK;
			} else if ((current_token.type == INT_LIT) || (current_token.type == VAR)) {
				/* expression parse will consume the first token */
				ret = expression_parse(tmp, current_token);
				if (ret) {
					goto cleanup;
				}
			} else {
				ERR_PRINT("Return statement not inside a function body");
				ret = COMP_ERR_SA;
				goto cleanup;
			}
		}
	} else if (current_token.type == R_CURLY) {
		ctx->last_token = R_CURLY;
		return COMP_OK;
	} else if ((current_token.type == INT_LIT) || (current_token.type == DECIMAL_LIT) || (current_token.type == STR_LIT)) {
		/* ex. 5; */
		ret = expression_parse(current_token, current_token);
		if (ret) {
			goto cleanup;
		}
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