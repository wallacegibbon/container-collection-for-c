#include "cc_binary_tree.h"
#include <stdlib.h>

int cc_binary_insert_child(struct cc_binary_node *parent, struct cc_binary_node *child, int on_left) {
	struct cc_binary_node **slot1, **slot2;

	if (parent == NULL)
		return 1;
	if (child == NULL)
		return 0;

	child->parent = parent;

	slot1 = on_left ? &parent->left : &parent->right;
	slot2 = on_left ? &child->left : &child->right;

	if (*slot1 != NULL)
		(*slot1)->parent = child;

	*slot2 = *slot1;
	*slot1 = child;

	return 0;
}

int cc_binary_node_add(struct cc_binary_node *self, void *data, int on_left) {
	struct cc_binary_node *node;
	node = malloc(sizeof(*node));
	if (node == NULL)
		goto fail1;

	node->parent = self;
	node->data = data;
	node->left = NULL;
	node->right = NULL;

	if (cc_binary_insert_child(self, node, on_left))
		goto fail2;

	return 0;

fail2:
	free(node);
fail1:
	return 1;
}

int cc_binary_tree_init(struct cc_binary_tree *self) {
	self->root.parent = NULL;
	self->root.left = NULL;
	self->root.right = NULL;
	self->root.size = 0;
	return 0;
}

struct cc_binary_tree *cc_binary_tree_new() {
	struct cc_binary_tree *self;

	self = malloc(sizeof(*self));
	if (self == NULL)
		goto fail1;

	if (cc_binary_tree_init(self))
		goto fail2;

	return self;

fail2:
	free(self);
fail1:
	return NULL;
}

/// Destroy the node tree recursively. Free all nodes except the root.
static int cc_binary_node_destroy(struct cc_binary_node *root, int depth) {
	if (root == NULL)
		return 0;

	if (cc_binary_node_destroy(root->left, depth + 1))
		return 1;
	if (cc_binary_node_destroy(root->right, depth + 1))
		return 2;

	if (depth > 0)
		free(root);

	return 0;
}

int cc_binary_tree_delete(struct cc_binary_tree *self) {
	int ret = 0;
	if (cc_binary_node_destroy(&self->root, 0))
		ret = 1;

	free(self);
	return ret;
}
