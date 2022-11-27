#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "syntactic_analysis.h"
#include "lexical_analysis.h"
#include "expression.h"
#include "symtab.h"
#include "compiler.h"
#include "generator.h"

extern struct compiler_ctx *ctx;
extern struct generator generator;

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

static void
generator_reverse_params()
{
	int i;
	struct params *aux;

	aux = malloc(generator.param_count * sizeof *aux);
	if (!aux) {
		exit(COMP_ERR_INTERNAL);
	}

	for (i = 0; i < generator.param_count; i++) {
		aux[generator.param_count - i - 1] = generator.params[i];
	}

	for (i = 0; i < generator.param_count; i++) {
		generator.params[i] = aux[i];
	}

	free(aux);
}

static void
generator_insert_param(struct lexeme token)
{

	switch (token.type) {
	case STR_LIT:
		generator.params[generator.param_count].is_constant = 1;
		generator.params[generator.param_count].value.str_val = token.value.str_val;
		generator.params[generator.param_count].type = STRING;
		break;

	case INT_LIT:
		generator.params[generator.param_count].is_constant = 1;
		generator.params[generator.param_count].value.int_val = token.value.int_val;
		generator.params[generator.param_count].type = INT;
		break;
	case DECIMAL_LIT:
		generator.params[generator.param_count].is_constant = 1;
		generator.params[generator.param_count].value.flt_val = token.value.flt_val;
		generator.params[generator.param_count].type = FLOAT;
		break;

	case VAR:
		generator.params[generator.param_count].is_constant = 0;
		generator.params[generator.param_count].value.var_name = token.id;
		generator.params[generator.param_count].type = UNKNOWN;
		break;

	case KEYWORD_NULL:
		generator.params[generator.param_count].is_constant = 0;
		generator.params[generator.param_count].value.var_name = NULL;
		generator.params[generator.param_count].type = T_NULL;
		break;

	default:
		/* should not happen */
		break;
	}

	generator.param_count++;
	if (generator.param_count > 10) {
		/* hard cap */
		exit(COMP_ERR_INTERNAL);
	}
}

static comp_err
rule_next_param(struct function_data data, int is_not_defined, struct function_data *new_data)
{
	struct lexeme current_token;
	static int param_counter = 1;
	int p_count = 0;

	if (!is_not_defined) {
		p_count = data.param_count;
	}

	current_token = getToken();
	if (current_token.type == R_PAR) {
		/* next_param -> empty */
		if ((param_counter != p_count) && (p_count != -1) && (!is_not_defined)) {
			ERR_PRINT("Received more parameters than expected.");
			return COMP_ERR_FUNC_PARAM;
		}
		data.param_count = param_counter;
		/*generatorPrepare(data);
		generatorPushParamString(param2);*/
		param_counter = 1;
		generator_reverse_params();
		return COMP_OK;
	} else if (current_token.type == COMMA) {
		/* next_param -> , Terminal next_param */
		current_token = getToken();

		/* check if its write */
		if ((p_count == -1) && ((current_token.type == STR_LIT) || (current_token.type == INT_LIT) || (current_token.type == DECIMAL_LIT)
		|| (current_token.type == VAR) || (current_token.type == KEYWORD_NULL))) {
			param_counter++;
			generator_insert_param(current_token);
			return rule_next_param(data, is_not_defined, new_data);
		}

		if (current_token.type == STR_LIT) {
			if (is_not_defined) {
				new_data->param_count++;
				new_data->params = realloc_func_params(new_data->params, new_data->param_count);
				if (!new_data->params) {
					return COMP_ERR_INTERNAL;
				}
				new_data->params[new_data->param_count - 1] = STRING;
			} else if ((data.params[param_counter] != STRING) && (data.params[param_counter] != QSTRING)) {
				ERR_PRINT("Unexpected string parameter.");
				return COMP_ERR_FUNC_PARAM;
			}
		} else if (current_token.type == INT_LIT) {
			if (is_not_defined) {
				new_data->param_count++;
				new_data->params = realloc_func_params(new_data->params, new_data->param_count);
				if (!new_data->params) {
					return COMP_ERR_INTERNAL;
				}
				new_data->params[new_data->param_count - 1] = INT;
			} else if ((data.params[param_counter] != INT) && (data.params[param_counter] != QINT)) {
				ERR_PRINT("Unexpected integer parameter.");
				return COMP_ERR_FUNC_PARAM;
			}
		} else if (current_token.type == DECIMAL_LIT) {
			if (is_not_defined) {
				new_data->param_count++;
				new_data->params = realloc_func_params(new_data->params, new_data->param_count);
				if (!new_data->params) {
					return COMP_ERR_INTERNAL;
				}
				new_data->params[new_data->param_count - 1] = FLOAT;
			} else if ((data.params[param_counter] != FLOAT) && (data.params[param_counter] != QFLOAT)) {
				ERR_PRINT("Unexpected decimal parameter.");
				return COMP_ERR_FUNC_PARAM;
			}
		} else if (current_token.type == VAR) {
			/* todo: code gen */
			if (is_not_defined) {
				new_data->param_count++;
				new_data->params = realloc_func_params(new_data->params, new_data->param_count);
				if (!new_data->params) {
					return COMP_ERR_INTERNAL;
				}
				new_data->params[new_data->param_count - 1] = UNKNOWN;
			}
		} else if (current_token.type == KEYWORD_NULL) {
			if (is_not_defined) {
				new_data->param_count++;
				new_data->params = realloc_func_params(new_data->params, new_data->param_count);
				if (!new_data->params) {
					return COMP_ERR_INTERNAL;
				}
				new_data->params[new_data->param_count - 1] = T_NULL;
			} else if ((data.params[param_counter] != QSTRING) && (data.params[param_counter] != QINT) && (data.params[param_counter] != QFLOAT)) {
				ERR_PRINT("Unexpected NULL parameter.");
				return COMP_ERR_FUNC_PARAM;
			}
		} else {
			return COMP_ERR_SA;
		}

		generator_insert_param(current_token);
		param_counter++;
		return rule_next_param(data, is_not_defined, new_data);
	} else {
		return COMP_ERR_SA;
	}
}

/* if the function is not defined yet, just count the params
 * and check it later when its defined */
static comp_err
rule_func_params(struct function_data data, int is_not_defined, char *function_name)
{
	int ret = COMP_OK, p_count = 0;
	struct lexeme current_token;
	struct bs_data *new_data = NULL;

	if (is_not_defined) {
		new_data = symtabSearch(ctx->global_sym_tab, function_name);
	} else {
		p_count = data.param_count;
	}

	current_token = getToken();
	if (current_token.type == R_PAR) {
		/* func_params -> empty */
		if (p_count) {
			ERR_PRINT("Expected more parameters in function call, but found 0.");
			ret = COMP_ERR_FUNC_PARAM;
			goto cleanup;
		}
		if (is_not_defined) {
			/* check later */
			new_data->data.fdata.param_count = 0;
		}

		/* generate no params */
		goto cleanup;
	} else if (!p_count && !is_not_defined) {
		ERR_PRINT("Expected 0 parameters in function call.");
		ret = COMP_ERR_FUNC_PARAM;
		goto cleanup;
	} else if (p_count == -1) {
		/* write */

		/* check variable definition */
		if (current_token.type == VAR) {
			if (ctx->in_function) {
				if (!symtabSearch(ctx->local_sym_tab, current_token.id)) {
					ERR_PRINT("Undefined variable in function call.");
					ret = COMP_ERR_UNDEF_VAR;
					goto cleanup;
				}
			} else {
				if (!symtabSearch(ctx->global_sym_tab, current_token.id)) {
					ERR_PRINT("Undefined variable in function call.");
					ret = COMP_ERR_UNDEF_VAR;
					goto cleanup;
				}
			}
		}

		generator_insert_param(current_token);
		ret = rule_next_param(data, 0, NULL);
		goto cleanup;
	}

	if (current_token.type == STR_LIT) {
		if (is_not_defined) {
			new_data->data.fdata.param_count = 1;
			new_data->data.fdata.params = realloc_func_params(new_data->data.fdata.params, new_data->data.fdata.param_count);
			if (!new_data->data.fdata.params) {
				ret = COMP_ERR_INTERNAL;
				goto cleanup;
			}
			new_data->data.fdata.params[0] = STRING;
		} else if ((data.params[0] != STRING) && (data.params[0] != QSTRING)) {
			ERR_PRINT("String parameter not expected.");
			ret = COMP_ERR_FUNC_PARAM;
			goto cleanup;
		}
	} else if (current_token.type == INT_LIT) {
		if (is_not_defined) {
			new_data->data.fdata.param_count = 1;
			new_data->data.fdata.params = realloc_func_params(new_data->data.fdata.params, new_data->data.fdata.param_count);
			if (!new_data->data.fdata.params) {
				ret = COMP_ERR_INTERNAL;
				goto cleanup;
			}
			new_data->data.fdata.params[0] = INT;
		} else if ((data.params[0] != INT) && (data.params[0] != QINT)) {
			ERR_PRINT("Integer parameter not expected.");
			ret = COMP_ERR_FUNC_PARAM;
			goto cleanup;
		}
	} else if (current_token.type == DECIMAL_LIT) {
		if (is_not_defined) {
			new_data->data.fdata.param_count = 1;
			new_data->data.fdata.params = realloc_func_params(new_data->data.fdata.params, new_data->data.fdata.param_count);
			if (!new_data->data.fdata.params) {
				ret = COMP_ERR_INTERNAL;
				goto cleanup;
			}
			new_data->data.fdata.params[0] = FLOAT;
		} else if ((data.params[0] != FLOAT) && (data.params[0] != QFLOAT)) {
			ERR_PRINT("Decimal parameter not expected.");
			ret = COMP_ERR_FUNC_PARAM;
			goto cleanup;
		}
	} else if (current_token.type == VAR) {
		/* check variable definition */
		if (ctx->in_function) {
			if (!symtabSearch(ctx->local_sym_tab, current_token.id)) {
				ERR_PRINT("Undefined variable in function call.");
				ret = COMP_ERR_UNDEF_VAR;
				goto cleanup;
			}
		} else {
			if (!symtabSearch(ctx->global_sym_tab, current_token.id)) {
				ERR_PRINT("Undefined variable in function call.");
				ret = COMP_ERR_UNDEF_VAR;
				goto cleanup;
			}
		}

		/* todo: runtime check */
		if (is_not_defined) {
			new_data->data.fdata.param_count = 1;
			new_data->data.fdata.params = realloc_func_params(new_data->data.fdata.params, new_data->data.fdata.param_count);
			if (!new_data->data.fdata.params) {
				ret = COMP_ERR_INTERNAL;
				goto cleanup;
			}
			/* unknown */
			new_data->data.fdata.params[0] = UNKNOWN;
		}
	} else if (current_token.type == KEYWORD_NULL) {
		if (is_not_defined) {
			new_data->data.fdata.param_count = 1;
			new_data->data.fdata.params = realloc_func_params(new_data->data.fdata.params, new_data->data.fdata.param_count);
			if (!new_data->data.fdata.params) {
				ret = COMP_ERR_INTERNAL;
				goto cleanup;
			}
			new_data->data.fdata.params[0] = T_NULL;
		} else if ((data.params[0] != QSTRING) && (data.params[0] != QINT) && (data.params[0] != QFLOAT)) {
			ERR_PRINT("Null parameter found in a function that does not accept it.");
			ret = COMP_ERR_FUNC_PARAM;
			goto cleanup;
		}
	} else if ((current_token.type == MINUS) || (current_token.type == PLUS)) {
		current_token = getToken();
		if ((current_token.type == INT_LIT) || (current_token.type == DECIMAL_LIT)) {
			ret = rule_next_param(data, 0, NULL);
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

	generator_insert_param(current_token);

	if (is_not_defined) {
		ret = rule_next_param(data, 1, &new_data->data.fdata);
	} else {
		ret = rule_next_param(data, 0, NULL);
	}

cleanup:
	return ret;
}

static comp_err
rule_func(struct function_data data, int is_not_defined, char *function_name)
{
	int ret = COMP_ERR_SA;
	struct lexeme current_token;

	current_token = getToken();
	if (current_token.type != L_PAR) {
		goto cleanup;
	}

	ret = rule_func_params(data, is_not_defined, function_name);
	if (ret != COMP_OK) {
		goto cleanup;
	}

	/* if all went well, ) should already be consumed */

	return COMP_OK;

cleanup:
	ERR_PRINT("Syntax error in function call.");
	return ret;
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

int
check_param(type param, char *param_str, struct function_data *data, int idx)
{
	int ret = 0;

	if (param == UNKNOWN) {
		data->params[idx] = token_get_param_type(param_str);
		goto cleanup;
	}

	if (!strcmp(param_str, "int")) {
		if (param != INT) {
			ret = 1;
			goto cleanup;
		}
	} else if (!strcmp(param_str, "float")) {
		if (param != FLOAT) {
			ret = 1;
			goto cleanup;
		}
	} else if (!strcmp(param_str, "string")) {
		if (param != STRING) {
			ret = 1;
			goto cleanup;
		}
	} else if (!strcmp(param_str, "?string")) {
		if ((param != STRING) || (param != T_NULL)) {
			ret = 1;
			goto cleanup;
		}
		data->params[idx] = QSTRING;
	} else if (!strcmp(param_str, "?float")) {
		if ((param != FLOAT) || (param != T_NULL)) {
			ret = 1;
			goto cleanup;
		}
		data->params[idx] = QFLOAT;
	} else if (!strcmp(param_str, "?int")) {
		if ((param != INT) || (param != T_NULL)) {
			ret = 1;
			goto cleanup;
		}
		data->params[idx] = QINT;
	}

cleanup:
	return ret;
}

int
check_undefined_functions()
{
	int i;

	for (i = 0; i < 20; i++) {
		if (ctx->unchecked_functions[i]) {
			ERR_PRINT("Undefined function found.");
			return COMP_ERR_UNDEF_FUNC;
		}
	}

	return COMP_OK;
}

static comp_err
rule_func_def_next_param(struct function_data *data, int check_params)
{
	struct lexeme current_token;
	static int p_counter = 1;
	struct bs_data *v_data;

	current_token = getToken();
	if (current_token.type == R_PAR) {
		if ((check_params) && (p_counter != data->param_count)) {
			ERR_PRINT("Function parameters don't match.");
			return COMP_ERR_FUNC_PARAM;
		}
		p_counter = 1;
		return COMP_OK;
	} else if (current_token.type == COMMA) {
		current_token = getToken();
		if (current_token.type == PARAM_TYPE) {
			/* next param */
			if (check_params) {
				p_counter++;
				if ((p_counter > data->param_count) || (check_param(data->params[p_counter - 1], current_token.id, data, p_counter - 1))) {
					ERR_PRINT("Function parameters don't match.");
					return COMP_ERR_FUNC_PARAM;
				}
			} else {
				data->param_count++;
				data->params = realloc_func_params(data->params, data->param_count);
				if (!data->params) {
					return COMP_ERR_INTERNAL;
				}
				data->params[data->param_count - 1] = token_get_param_type(current_token.id);
			}

			current_token = getToken();
			if (current_token.type == VAR) {
				if (dataInit(&v_data)) {
					return COMP_ERR_INTERNAL;
				}
				v_data->data.vdata.is_defined = 1;

				if (symtabInsert(&ctx->local_sym_tab, current_token.id, v_data)) {
					return COMP_ERR_INTERNAL;
				}
				return rule_func_def_next_param(data, check_params);
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
rule_func_def_parameters(struct function_data *data, int check_params)
{
	int ret = COMP_OK;
	struct lexeme current_token;
	struct bs_data *v_data;

	current_token = getToken();
	if (current_token.type == R_PAR) {
		/* no params */
		data->param_count = 0;
		goto cleanup;
	} else if (current_token.type == PARAM_TYPE) {
		if (check_params) {
			if ((!data->param_count) || (check_param(data->params[0], current_token.id, data, 0))) {
				ERR_PRINT("Function parameters don't match.");
				ret = COMP_ERR_FUNC_PARAM;
				goto cleanup;
			}
		} else {
			/* insert new param */
			data->param_count++;
			data->params = realloc_func_params(data->params, data->param_count);
			if (!data->params) {
				ret = COMP_ERR_INTERNAL;
				goto cleanup;
			}
			data->params[0] = token_get_param_type(current_token.id);
		}

		current_token = getToken();
		if (current_token.type == VAR) {
			/* insert into symtab */
			if (dataInit(&v_data)) {
				ret = COMP_ERR_INTERNAL;
				goto cleanup;
			}
			v_data->data.vdata.is_defined = 1;

			if (symtabInsert(&ctx->local_sym_tab, current_token.id, v_data)) {
				ret = COMP_ERR_INTERNAL;
				goto cleanup;
			}

			ret = rule_func_def_next_param(data, check_params);
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

int
check_not_complete_record(char *function_name)
{
	int i;

	for (i = 0; i < 20; i++) {
		if (ctx->unchecked_functions[i]) {
			if (!strcmp(ctx->unchecked_functions[i], function_name)) {
				return 0;
			}
		}
	}

	return 1;
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
	int param_check = 0;

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
		rc = check_not_complete_record(current_token.id);
		if (!rc) {
			/* not complete record found, check params */
			param_check = 1;
		} else {
			/* redefiniton found */
			ret = COMP_ERR_UNDEF_FUNC;
			ERR_PRINT("Function redefinition attempted.");
			goto cleanup;
		}
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

	ret = rule_func_def_parameters(&(data->data.fdata), param_check);
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
		ctx->seen_return = 1;
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

	ret = rule_statement_list(data);
	if (ret != COMP_OK) {
		goto cleanup;
	}

	/* if all went well, } should already be consumed */
	if (!ctx->seen_return) {
		ERR_PRINT("Missing return value in non-void function.");
		ret = COMP_ERR_FUNC_PARAM;
		goto cleanup;
	}

	if (param_check) {
		ctx->unchecked_functions[rc] = NULL;
	}
	ctx->seen_return = 0;
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

	ret = expression_parse(current_token, current_token);
	if (ret == COMP_ERR_UNDEF_VAR) {
		ERR_PRINT("Undefined variable in if statement.");
		return COMP_ERR_UNDEF_VAR;

	} else if (ret == COMP_ERR_MISMATCHED_TYPES) {
		ERR_PRINT("Mismatched types in if statement.");
		return COMP_ERR_MISMATCHED_TYPES;

	} else if (ret == COMP_ERR_SA) {
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
rule_var_declaration(char *var_name)
{
	struct lexeme current_token;
	struct lexeme next_token;
	int ret = COMP_OK, rc;
	struct bs_data *data, *v_data;

	current_token = getToken();
	if (current_token.type == ASSIGNMENT) {
		/* insert variable into symtab */
		rc = dataInit(&v_data);
		if (rc) {
			ret = COMP_ERR_INTERNAL;
			goto cleanup;
		}
		v_data->data.vdata.is_defined = 1;
		if (ctx->in_function) {
			rc = symtabInsert(&ctx->local_sym_tab, var_name, v_data);
		} else {
			// info to generator
			if(defvar_global(var_name)){
				ret = COMP_ERR_INTERNAL;
				goto cleanup;
			}
			rc = symtabInsert(&ctx->global_sym_tab, var_name, v_data);
		}
		if (rc) {
			ret = COMP_ERR_INTERNAL;
			goto cleanup;
		}

		next_token = getToken();
		if (next_token.type == FUN_ID) {
			/* save function name in generator */
			generator.function_name = next_token.id;
			//printf("%s\n", next_token.id);
			/* search for the function */
			data = symtabSearch(ctx->global_sym_tab, next_token.id);
			if (!data) {
				ret = dataInit(&data);
				if (ret) {
					ret = COMP_ERR_INTERNAL;
					goto cleanup;
				}
				data->is_function = 1;
				data->data.fdata.is_defined = 0;
				ret = symtabInsert(&ctx->global_sym_tab, next_token.id, data);
				if (ret) {
					ret = COMP_ERR_INTERNAL;
					goto cleanup;
				}
				ctx->unchecked_functions[ctx->empty_index] = next_token.id;
				ctx->empty_index++;
				ret = rule_func(data->data.fdata, 1, next_token.id);
			} else {
				ret = rule_func(data->data.fdata, 0, next_token.id);
			}
			generatorExecute();
			generator_reset();

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
			ret = expression_parse(current_token, next_token);
			if (ret == COMP_ERR_UNDEF_VAR) {
				ERR_PRINT("Undefined variable in variable declaration.");
				return COMP_ERR_UNDEF_VAR;

			} else if (ret == COMP_ERR_MISMATCHED_TYPES) {
				ERR_PRINT("Mismatched types in variable declaration.");
				return COMP_ERR_MISMATCHED_TYPES;

			} else if (ret == COMP_ERR_SA) {
				goto cleanup;
			} else {
				return COMP_OK;
			}
		}
	} else if (current_token.type == SEMICOLON) {
		/* check definition */
		if (ctx->in_function) {
			v_data = symtabSearch(ctx->local_sym_tab, var_name);
		} else {
			v_data = symtabSearch(ctx->global_sym_tab, var_name);
		}

		if (!v_data) {
			ERR_PRINT("Undefined variable");
			return COMP_ERR_UNDEF_VAR;
		}
		return COMP_OK;
	} else {
		goto cleanup;
	}

cleanup:
	ERR_PRINT("Syntax error in variable declaration");
	return ret;
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

	ret = expression_parse(current_token, current_token);
	if (ret == COMP_ERR_UNDEF_VAR) {
		ERR_PRINT("Undefined variable in while statement.");
		return COMP_ERR_UNDEF_VAR;

	} else if (ret == COMP_ERR_MISMATCHED_TYPES) {
		ERR_PRINT("Mismatched types in while statement.");
		return COMP_ERR_MISMATCHED_TYPES;

	} else if (ret == COMP_ERR_SA) {
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
rule_return(struct function_data data)
{
	struct lexeme current_token = {0};
	struct lexeme next_token;
	int ret;

	next_token = getToken();
	if (next_token.type != SEMICOLON) {
		ret = expression_parse(current_token, next_token);
		if (ret == COMP_ERR_UNDEF_VAR) {
			ERR_PRINT("Undefined variable in return.");
			return COMP_ERR_UNDEF_VAR;

		} else if (ret == COMP_ERR_MISMATCHED_TYPES) {
			ERR_PRINT("Mismatched types in return.");
			return COMP_ERR_MISMATCHED_TYPES;

		} else if (ret == COMP_ERR_SA) {
			ERR_PRINT("Syntax error in return");
			return COMP_ERR_SA;
		}
	} else {
		/* it's return; -> fun has to have void return */
		if (data.return_type != VOID) {
			ERR_PRINT("Missing return value in non-void function.");
			return COMP_ERR_FUNC_RETURN;
		}
	}

	ctx->seen_return = 1;
	return COMP_OK;
}

/* rule: program -> prolog statement_list EOF */
static comp_err
rule_statement_list(struct bs_data *data)
{
	int ret;
	struct lexeme current_token;
	struct lexeme tmp = {0};
	struct function_data empty = {0};

	/* parse lexemes that can appear in the global scope */
	ctx->last_token = -1;
	current_token = getToken();
	if (current_token.type == FUN_ID) {
		/* save function name in generator */
		generator.function_name = current_token.id;
		//printf("where is%s\n", current_token.id); // to delete

		/* search for the function */
		data = symtabSearch(ctx->global_sym_tab, current_token.id);
		if (!data) {
			ctx->unchecked_functions[ctx->empty_index] = current_token.id;
			ctx->empty_index++;
			ret = rule_func(empty, 1, current_token.id);
		} else {
			ret = rule_func(data->data.fdata, 0, current_token.id);
		}

		generatorExecute();
		generator_reset();

		if (ret != COMP_OK) {
			goto cleanup;
		}
		current_token = getToken();
		if (current_token.type != SEMICOLON) {
			ret = COMP_ERR_SA;
			goto cleanup;
		}
	} else if (current_token.type == VAR) {
		ret = rule_var_declaration(current_token.id);
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

		ret = expression_parse(current_token, current_token);
		if (ret == COMP_ERR_UNDEF_VAR) {
			ERR_PRINT("Undefined variable."); // tady nevim, jak to upresnit
			return COMP_ERR_UNDEF_VAR;

		} else if (ret == COMP_ERR_MISMATCHED_TYPES) {
			ERR_PRINT("Mismatched types.");  // to stejne tady
			return COMP_ERR_MISMATCHED_TYPES;

		} else if (ret == COMP_ERR_SA) {
			goto cleanup;

		} else {
			return COMP_OK;
		}

	} else if (current_token.type == SEMICOLON) {
		return COMP_OK;
	} else if (current_token.type == COMMENT) {
		return rule_statement_list(data);
	} else if (current_token.type == PROLOG_END) {
		ret = rule_prolog_end();
		if (ret != COMP_OK) {
			goto cleanup;
		}
	} else if (current_token.type == LEX_EOF) {
		//here write code
		return COMP_OK;
	} else if (current_token.type == KEYWORD_RETURN) {
		if (ctx->in_function) {
			ret = rule_return(data->data.fdata);
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
				if (ret == COMP_ERR_UNDEF_VAR) {
					ERR_PRINT("Undefined variable."); // stejne
					return COMP_ERR_UNDEF_VAR;

				} else if (ret == COMP_ERR_MISMATCHED_TYPES) {
					ERR_PRINT("Mismatched types.");  // stejne zas
					return COMP_ERR_MISMATCHED_TYPES;

				} else if (ret == COMP_ERR_SA) {
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
		int ret = expression_parse(current_token, current_token);
		if (ret == COMP_ERR_UNDEF_VAR) {
			ERR_PRINT("Undefined variable."); // zase
			return COMP_ERR_UNDEF_VAR;

		} else if (ret == COMP_ERR_MISMATCHED_TYPES) {
			ERR_PRINT("Mismatched types.");  // zase
			return COMP_ERR_MISMATCHED_TYPES;

		} else if (ret == COMP_ERR_SA) {
			goto cleanup;
		}
	} else {
		return COMP_ERR_SA;
	}

	return rule_statement_list(data);

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

	ret = rule_statement_list(NULL);
	if (ret) {
		goto cleanup;
	}

cleanup:
	return ret;
}
