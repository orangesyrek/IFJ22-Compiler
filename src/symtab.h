#ifndef TREE_H
#define TREE_H

#include <stdint.h>

struct bs_tree {
	uint32_t key;
	void *data;
	struct bs_tree *left;
	struct bs_tree *right;
};

struct bs_data{
	uint32_t id;

	int is_function;
	union {
		struct function_data;
		struct variable_data;
	} data;

};

struct function_data{
type return_type;
int is_defined;
//list of parameters
};

struct variable_data{
int is_init;
type data_type;
int is_used;
}

#endif
