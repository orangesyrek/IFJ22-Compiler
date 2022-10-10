#ifndef COMPILER_H
#define COMPILER_H

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
} COMP_ERR;

struct compiler_ctx {
	int last_read;
	unsigned int current_row;
	int strict_types;
	struct bs_tree **sym_tab;
	unsigned int frame_count;
};

int compiler_ctx_new(struct compiler_ctx **ctx);

void compiler_ctx_destroy(struct compiler_ctx *ctx);

#endif
