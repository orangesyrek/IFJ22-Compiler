#ifndef COMPILER_H
#define COMPILER_H

#define ERR_PRINT(msg) \
	fprintf(stderr, "Error (%s) occured on php line %u, code line %d, in function %s.\n", msg, ctx->current_row, __LINE__, __func__)

typedef enum {
	COMP_OK = 0,
	COMP_ERR_LA,				/**< unexpected lexeme */
	COMP_ERR_SA,				/**< syntax error */
	COMP_ERR_UNDEF_FUNC,		/**< undefined func or redefinition */
	COMP_ERR_FUNC_PARAM,		/**< incorrect amount of func parameters/wrong types or unexpected return type */
	COMP_ERR_UNDEF_VAR,			/**< use of an undefined variable */
	COMP_ERR_FUNC_RETURN,		/**< missing/unexpected return statement */
	COMP_ERR_MISMATCHED_TYPES,	/**< error in type compatibility in statements */
	COMP_ERR_SEMANTIC,			/**< other semantic error */
	COMP_ERR_INTERNAL = 99 		/**< mem allocation err, etc. */
} comp_err;

struct compiler_ctx {
	int last_read;
	int in_function;
	int seen_return;
	unsigned int current_row;
	int last_token;
	struct bs_tree *global_sym_tab;
	struct bs_tree *local_sym_tab;
};

int compiler_ctx_new(struct compiler_ctx **ctx);

void compiler_ctx_destroy(struct compiler_ctx *ctx);

#endif
