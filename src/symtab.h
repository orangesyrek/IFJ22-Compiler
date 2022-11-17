#ifndef TREE_H
#define TREE_H

#include <stdint.h>

typedef enum{
	STRING,
	INT,
	FLOAT,
	BOOL,
	NULL, // idk if not rename
	VOID,
	QSTRING,
	QINT,
	QFLOAT
} type;

struct bs_tree {
	char *key;
	void *data;
	struct bs_tree *left;
	struct bs_tree *right;
};

struct function_data{
	type return_type;
	int is_defined;
	int *params;
	int paramcount;
};

struct variable_data{
	int is_init;
	type data_type;
	int is_used;
};
struct bs_data{
	uint32_t id;

	int is_function;
	union {
		struct function_data fdata;
		struct variable_data vdata;
	} data;

};


void symtabInit(struct bs_tree *t);

#endif
