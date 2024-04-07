#ifndef __CC_BINARY_TREE_H
#define __CC_BINARY_TREE_H

#ifdef NO_MALLOC
#error "You can NOT use binary tree without support for the `malloc` function."
#endif

#include "cc_iter.h"
#include <stddef.h>

struct cc_binary_node {
	struct cc_binary_node *parent;
	struct cc_binary_node *left;
	struct cc_binary_node *right;
	union {
		void *data;
		size_t size;
	};
};

int cc_binary_insert_child(struct cc_binary_node *parent, struct cc_binary_node *child, int on_left);
int cc_binary_node_add(struct cc_binary_node *self, void *data, int on_left);

struct cc_binary_tree {
	struct cc_binary_node root;
};

struct cc_binary_tree *cc_binary_tree_new();
int cc_binary_tree_delete(struct cc_binary_tree *self);

struct cc_binary_tree_iter {
	struct cc_iter_i *iterator;
	struct cc_binary_tree *tree;
};

int cc_binary_tree_iter_init(struct cc_binary_tree_iter *self, struct cc_binary_tree *tree);
int cc_binary_tree_iter_next(struct cc_binary_tree_iter *self, void **item, size_t *index);

#endif
