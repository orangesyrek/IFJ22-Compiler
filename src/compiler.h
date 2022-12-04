/*
 * FIT VUT 2022 - IFJ Project
 * Implementation of a compiler for an imperative language IFJ22
 *
 * File: compiler.h
 * Author(s): xjanot04
 */

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

/**
 * @brief Structure holding current compiler context.
 */
struct compiler_ctx {
	int last_read;	/**< last character read from input */
	int in_function;	/**< sets the context to function */
	int seen_return;	/**< indicates if return in current function was seen */
	int curr_fun_ret_type;	/**< hold current function's return type */
	char *unchecked_functions[20];	/**< array storing up to 20 undefined functions names */
	int empty_index;	/**< unchecked_functions array's empty index */
	unsigned int current_row;	/**< current row being read from input */
	int last_token;		/**< last type of token read */
	struct bs_tree *global_sym_tab;	/**< global table of symbols */
	struct bs_tree *local_sym_tab;	/**< local table of symbols */
};

/**
 * @brief Initializes compiler context.
 *
 * @param ctx Pointer to the context.
 * @return 0 on success, 1 otherwise.
 */
int compiler_ctx_new(struct compiler_ctx **ctx);

/**
 * @brief Free the compiler context.
 *
 * @param ctx Pointer to the context.
 */
void compiler_ctx_destroy(struct compiler_ctx *ctx);

#endif
