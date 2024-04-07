#include "cc_binary_tree.h"
#include <assert.h>

int main() {
	struct cc_binary_tree *tree;

	tree = cc_binary_tree_new();
	assert(tree != NULL);

	assert(!cc_binary_node_add(&tree->root, (void **)1, 1));
	assert(!cc_binary_node_add(&tree->root, (void **)2, 1));
	assert(!cc_binary_node_add(&tree->root, (void **)3, 1));
	assert(!cc_binary_node_add(&tree->root, (void **)4, 0));
	assert(!cc_binary_node_add(&tree->root, (void **)5, 0));

	assert(!cc_binary_tree_delete(tree));

	return 0;
}
