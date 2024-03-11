#include "cc_list.h"
#include "cc_common.h"
#include <stdint.h>
#include <stdlib.h>

void cc_list_node_delete(struct cc_list_node *self, cc_handle_fn cleanup_fn) {
	if (cleanup_fn != NULL)
		cleanup_fn(self->p_data);

	free(self);
}

int cc_list_add(struct cc_list *self, void *value) {
	struct cc_list_node *node;
	node = malloc(sizeof(*node));
	if (node == NULL)
		return 0;

	node->p_data = value;

	self->root.prev->next = node;
	node->prev = self->root.prev;

	self->root.prev = node;
	node->next = &self->root;

	self->root.size++;
	return 1;
}

/// Caution: You may need to delete `right` list after this concatenation.
int cc_list_concat(struct cc_list *left, struct cc_list *right) {
	if (left == NULL)
		return 0;
	if (right == NULL)
		return 1;

	left->root.prev->next = right->root.next;
	right->root.next->prev = left->root.prev;

	left->root.prev = right->root.prev;
	right->root.prev->next = &left->root;

	left->root.size += right->root.size;

	cc_list_init(right);
	return 1;
}

static struct cc_list_node *prev_node_of(struct cc_list *self, size_t index) {
	struct cc_list_node *node;
	node = &self->root;
	while (index--)
		node = node->next;

	return node;
}

int cc_list_insert(struct cc_list *self, void *value, size_t index) {
	struct cc_list_node *entry, *node;
	if (index > self->root.size)
		return 0;

	node = malloc(sizeof(*node));
	if (node == NULL)
		return 0;

	node->p_data = value;

	entry = prev_node_of(self, index);
	node->next = entry->next;
	node->prev = entry;
	entry->next->prev = node;
	entry->next = node;

	self->root.size++;
	return 1;
}

int cc_list_remove(struct cc_list *self, size_t index, cc_handle_fn cleanup_fn) {
	struct cc_list_node *node, *node_to_remove;
	if (index >= self->root.size)
		return 0;

	node = prev_node_of(self, index);

	node_to_remove = node->next;
	node->next->next->prev = node;
	node->next = node->next->next;

	cc_list_node_delete(node_to_remove, cleanup_fn);

	self->root.size--;
	return 1;
}

void cc_list_init(struct cc_list *self) {
	self->root.prev = &self->root;
	self->root.next = &self->root;
	self->root.size = 0;
}

struct cc_list *cc_list_new() {
	struct cc_list *self;
	self = malloc(sizeof(*self));
	if (self == NULL)
		return NULL;

	cc_list_init(self);
	return self;
}

static inline struct cc_list_node *free_and_next(struct cc_list_node *current, cc_handle_fn cleanup_fn) {
	struct cc_list_node *next;
	next = current->next;
	cc_list_node_delete(current, cleanup_fn);
	return next;
}

void cc_list_delete(struct cc_list *self, cc_handle_fn cleanup_fn) {
	struct cc_list_node *node;

	node = self->root.next;

	while (node != &self->root)
		node = free_and_next(node, cleanup_fn);

	free(self);
}

static int cc_list_iter_next(struct cc_list_iter *self, uintptr_t *value);

static const struct cc_iter_i iterator_interface = {
	.next = (cc_iter_next_fn)cc_list_iter_next,
};

static inline void cc_list_iter_step(struct cc_list_iter *self) {
	if (self->direction == 0)
		self->cursor = self->cursor->next;
	else
		self->cursor = self->cursor->prev;
}

int cc_list_iter_init(struct cc_list_iter *self, struct cc_list *list, uint8_t direction) {
	if (list == NULL)
		return 0;

	self->iterator = (struct cc_iter_i *)&iterator_interface;
	self->list = list;
	self->direction = direction;
	self->cursor = &self->list->root;
	cc_list_iter_step(self);

	return 1;
}

static int cc_list_iter_next(struct cc_list_iter *self, uintptr_t *value) {
	if (self->cursor == &self->list->root)
		return 0;

	*value = (uintptr_t)self->cursor->data;
	cc_list_iter_step(self);

	return 1;
}
