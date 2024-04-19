#include "cc_list.h"
#include "cc_common.h"
#include <stdlib.h>

static struct cc_list_node *prev_node_of(struct cc_list *self, size_t index) {
	struct cc_list_node *node;

	node = &self->root;
	while (index--)
		node = node->next;

	return node;
}

int cc_list_insert(struct cc_list *self, size_t index, void *value) {
	struct cc_list_node *entry, *node;

	if (index > self->root.size)
		return 1;

	node = malloc(sizeof(*node));
	if (node == NULL)
		return 2;

	node->data = value;

	entry = prev_node_of(self, index);
	node->next = entry->next;
	node->prev = entry;
	entry->next->prev = node;
	entry->next = node;

	self->root.size++;
	return 0;
}

int cc_list_remove(struct cc_list *self, size_t index, void **result) {
	struct cc_list_node *node, *node_to_remove;

	/// You have to provide `result`, or the `node_to_remove->data` may leak.
	if (try_reset_double_p(result))
		return 1;
	if (index >= self->root.size)
		return 2;

	node = prev_node_of(self, index);

	node_to_remove = node->next;
	node->next->next->prev = node;
	node->next = node->next->next;

	*result = node_to_remove->data;
	free(node_to_remove);

	self->root.size--;
	return 0;
}

int cc_list_append(struct cc_list *self, void *value) {
	struct cc_list_node *node;

	node = malloc(sizeof(*node));
	if (node == NULL)
		return 1;

	node->data = value;
	self->root.prev->next = node;
	node->prev = self->root.prev;

	self->root.prev = node;
	node->next = &self->root;

	self->root.size++;
	return 0;
}

int cc_list_init(struct cc_list *self);

/// Caution: You may need to delete `right` list after this concatenation.
int cc_list_concat(struct cc_list *left, struct cc_list *right) {
	/// `left` can not be NULL since it holds the result.
	if (left == NULL)
		return 1;
	if (right == NULL)
		return 0;

	left->root.prev->next = right->root.next;
	right->root.next->prev = left->root.prev;

	left->root.prev = right->root.prev;
	right->root.prev->next = &left->root;

	left->root.size += right->root.size;

	if (cc_list_init(right))
		return 2;

	return 0;
}

int cc_list_init(struct cc_list *self) {
	self->root.prev = &self->root;
	self->root.next = &self->root;
	self->root.size = 0;
	return 0;
}

struct cc_list *cc_list_new() {
	struct cc_list *self;

	self = malloc(sizeof(*self));
	if (self == NULL)
		goto fail1;

	if (cc_list_init(self))
		goto fail2;

	return self;

fail2:
	free(self);
fail1:
	return NULL;
}

static inline struct cc_list_node *free_and_next(struct cc_list_node *current) {
	struct cc_list_node *next;
	next = current->next;
	free(current);
	return next;
}

int cc_list_delete(struct cc_list *self) {
	struct cc_list_node *node;

	node = self->root.next;
	while (node != &self->root)
		node = free_and_next(node);

	free(self);
	return 0;
}

static struct cc_iter_i iterator_interface = {
	.next = (cc_iter_next_fn_t)cc_list_iter_next,
};

static inline void cc_list_iter_step(struct cc_list_iter *self) {
	if (self->direction == 0)
		self->cursor = self->cursor->next;
	else
		self->cursor = self->cursor->prev;
}

int cc_list_iter_next(struct cc_list_iter *self, void **item, size_t *index) {
	if (try_reset_double_p(item))
		return 1;
	if (self->cursor == &self->list->root)
		return 2;

	*item = &self->cursor->data;

	if (index != NULL)
		*index = self->index;

	self->index++;

	cc_list_iter_step(self);
	return 0;
}

int cc_list_iter_init(struct cc_list_iter *self, struct cc_list *list, int direction) {
	if (list == NULL)
		return 1;

	self->iterator = &iterator_interface;
	self->list = list;
	self->index = 0;
	self->direction = direction;
	self->cursor = &self->list->root;

	cc_list_iter_step(self);
	return 0;
}
