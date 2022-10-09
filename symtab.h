#ifndef TREE_H
#define TREE_H

#include <stdint.h>

struct bs_tree {
	uint32_t key;
	void *data;
	struct bs_tree *left;
	struct bs_tree *right;
};


#endif
