/*
 * FIT VUT 2022 - IFJ Project
 * Implementation of a compiler for an imperative language IFJ22
 *
 * File: symtab.h
 * Author(s): xvalik04
 */

#ifndef TREE_H
#define TREE_H

/*Types of variables */
typedef enum{
	STRING,
	INT,
	FLOAT,
	VOID,
	QSTRING,
	QINT,
	QFLOAT,
	UNKNOWN,
	T_NULL
} type;

/*Binary tree*/
struct bs_tree {
	const char *key;
	struct bs_data *data;
	struct bs_tree *left;
	struct bs_tree *right;
};

struct function_data{
	type return_type;
	int is_defined;
	type *params;
	int param_count;
};

struct variable_data{
	int is_defined;
	type data_type;
};

struct bs_data{
	int is_function;
	union {
		struct function_data fdata;
		struct variable_data vdata;
	} data;

};

/*
Inicialization of symTable
@param bs_tree pointer to binary tree
*/
void symtabInit(struct bs_tree **t);

/*
Insert to symTable
@param bs_tree pointer to binary tree
@param key key of node
@param bs_data pointer to node data
*/

int symtabInsert(struct bs_tree **t, const char *key, struct bs_data *data);

/*
Search in  symTable
@param bs_tree pointer to binary tree
@param key key of node
*/

struct bs_data* symtabSearch(struct bs_tree *t, char *key);


/*
Delete all nodes in symTable
@param bs_tree pointer to binary tree
*/

void symtabDispose(struct bs_tree **t);
/*
Inicialization of data
@param bs_data pointer to data
*/

int dataInit(struct bs_data **data);

/*
 Prints the tree
 @param t BST pointer
 */
void printTree(struct bs_tree *t);

/**
 * @brief Insert built-in functions into a tree.
 *
 * @param tree Pointer to the tree.
 * @return 0 on success, non-zero otherwise.
 */
int insert_builtin_functions(struct bs_tree **tree);

/**
 * @brief Reallocate the function's parameters array.
 *
 * @param params Pointer to the array.
 * @param param_count Parameter count.
 * @return The reallocated array on success, NULL on failure.
 */
type * realloc_func_params(type *params, int param_count);

#endif
