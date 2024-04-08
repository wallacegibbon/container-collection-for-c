#include "cc_binary_tree.h"
#include "cc_common.h"
#include "cc_list.h"
#include <stdarg.h>
#include <stdlib.h>

struct cc_binary_node *cc_binary_node_new(struct cc_binary_node *parent, void *data) {
	struct cc_binary_node *self;
	self = malloc(sizeof(*self));
	if (self == NULL)
		return NULL;

	self->parent = parent;
	self->data = data;
	self->left = NULL;
	self->right = NULL;
	return self;
}

int cc_binary_node_insert_left(struct cc_binary_node *self, void *data) {
	struct cc_binary_node *node;
	node = cc_binary_node_new(self, data);
	if (node == NULL)
		return 1;

	if (self->left != NULL)
		self->left->parent = node;

	node->left = self->left;
	self->left = node;
	return 0;
}

int cc_binary_node_insert_right(struct cc_binary_node *self, void *data) {
	struct cc_binary_node *node;
	node = cc_binary_node_new(self, data);
	if (node == NULL)
		return 1;

	if (self->right != NULL)
		self->right->parent = node;

	node->right = self->right;
	self->right = node;
	return 0;
}

int cc_binary_node_rotate_left(struct cc_binary_node **start_slot) {
	struct cc_binary_node *start = *start_slot;

	if (start == NULL)
		return 0;
	if (start->right == NULL)
		return 1;

	*start_slot = start->right;
	start->right->parent = start->parent;

	start->parent = start->right;

	if (start->right->left != NULL)
		start->right->left->parent = start;

	start->right = start->right->left;
	(*start_slot)->left = start;

	return 0;
}

int cc_binary_node_rotate_right(struct cc_binary_node **start_slot) {
	struct cc_binary_node *start = *start_slot;

	if (start == NULL)
		return 0;
	if (start->left == NULL)
		return 1;

	*start_slot = start->left;
	start->left->parent = start->parent;

	start->parent = start->left;

	if (start->left->right != NULL)
		start->left->right->parent = start;

	start->left = start->left->right;
	(*start_slot)->right = start;

	return 0;
}

int cc_binary_node_print(struct cc_binary_node *root, int depth) {
	int tmp = 0;
	cc_print_n("\t", depth);
	if (root == NULL) {
		cc_debug_print("<NULL>\n");
		return 0;
	}
	cc_debug_print("%d\n", root->data);
	tmp |= cc_binary_node_print(root->right, depth + 1);
	tmp |= cc_binary_node_print(root->left, depth + 1);
	return tmp;
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

static struct cc_iter_i iterator_interface = {
	.next = (cc_iter_next_fn)cc_binary_tree_iter_next,
};

static int iter_queue_add(struct cc_binary_tree_iter *self, void *data) {
	if (data == NULL)
		return 0;
	if (self->direction == CC_TRAVERSE_DEPTH_LEFT || self->direction == CC_TRAVERSE_DEPTH_RIGHT)
		return cc_list_insert(self->queue, 0, data);
	else
		return cc_list_append(self->queue, data);
}

static int iter_queue_add_multi(struct cc_binary_tree_iter *self, int n, ...) {
	va_list args;
	va_start(args, n);
	while (n-- > 0) {
		if (iter_queue_add(self, va_arg(args, struct cc_binary_node *)))
			return 1;
	}
	va_end(args);
	return 0;
}

static int iter_queue_add_child(struct cc_binary_tree_iter *self, struct cc_binary_node *node) {
	int tmp = 0;

	if (node == NULL)
		return 0;

	if (self->direction == CC_TRAVERSE_DEPTH_LEFT || self->direction == CC_TRAVERSE_BREADTH_RIGHT)
		tmp = iter_queue_add_multi(self, 2, node->right, node->left);
	else if (self->direction == CC_TRAVERSE_DEPTH_RIGHT || self->direction == CC_TRAVERSE_BREADTH_LEFT)
		tmp = iter_queue_add_multi(self, 2, node->left, node->right);
	else
		tmp = 2;

	return tmp;
}

struct cc_binary_tree_iter *cc_binary_tree_iter_new(struct cc_binary_tree *tree, enum cc_traverse_direction direction) {
	struct cc_binary_tree_iter *self;
	struct cc_list *queue;

	if (tree == NULL)
		goto fail1;

	self = malloc(sizeof(*self));
	if (self == NULL)
		goto fail1;

	self->queue = cc_list_new();
	if (self->queue == NULL)
		goto fail2;
	if (cc_list_append(self->queue, tree))
		goto fail3;

	self->iterator = &iterator_interface;
	self->index = 0;
	self->direction = direction;

	return self;

fail3:
	cc_list_delete(self->queue);
fail2:
	free(self);
fail1:
	return NULL;
}

int cc_binary_tree_iter_delete(struct cc_binary_tree_iter *self) {
	if (cc_list_delete(self->queue))
		return 1;
	free(self);
	return 0;
}

int cc_binary_tree_iter_next(struct cc_binary_tree_iter *self, void **item, size_t *index) {
	struct cc_binary_node *current;
	int tmp;

	if (cc_list_remove(self->queue, 0, (void **)&current))
		return 1;

	*item = &current->data;

	if (iter_queue_add_child(self, current))
		return 3;

	if (index != NULL)
		*index = self->index;

	self->index++;
	return 0;
}
