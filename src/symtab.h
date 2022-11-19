#ifndef TREE_H
#define TREE_H


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*Types of variables */
typedef enum{
	STRING,
	INT,
	FLOAT,
	BOOL,
	TNULL, // idk if not rename
	VOID,
	QSTRING,
	QINT,
	QFLOAT
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
	int is_init;
	type data_type;
	int is_used;
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

void printTree(struct bs_tree *t);

/**
 * Insert builtin functions
*/
int insert_builtin_functions(struct bs_tree **tree);

/**
 * Reallocate parameters array
 */
type * realloc_func_params(type *params, int param_count);

#endif
