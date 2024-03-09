#include "cc_list.h"
#include "cc_common.h"
#include <stdint.h>
#include <stdlib.h>

void cc_list_node_init(struct cc_list_node *self, void *value) {
	self->p_data = value;
	self->next = NULL;
	self->prev = NULL;
}

struct cc_list_node *cc_list_node_new(void *value) {
	struct cc_list_node *self;
	self = malloc(sizeof(*self));
	if (self == NULL)
		return NULL;

	cc_list_node_init(self, value);
	return self;
}

void cc_list_node_delete(struct cc_list_node *self, cc_handle_fn cleanup_fn) {
	if (cleanup_fn != NULL)
		cleanup_fn(self->p_data);

	free(self);
}

int cc_list_add(struct cc_list *self, void *value) {
	struct cc_list_node *node;
	node = cc_list_node_new(value);
	if (node == NULL)
		return 0;

	self->root.prev->next = node;
	node->prev = self->root.prev;

	self->root.prev = node;
	node->next = &self->root;

	self->root.size++;
	return 1;
}

int cc_list_concat(struct cc_list *left, struct cc_list *right) {
	if (left == NULL)
		return 0;
	if (right == NULL)
		return 1;

	left->root.prev->next = right->root.next;
	right->root.next->prev = left->root.prev;

	left->root.prev = right->root.prev;
	right->root.prev->next = &left->root;

	right->root.next = NULL;
	right->root.prev = NULL;

	left->root.size += right->root.size;
	right->root.size = 0;

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

static void cc_list_iter_step(struct cc_list_iter *self) {
	if (self->direction == 0)
		self->cursor = self->cursor->next;
	else
		self->cursor = self->cursor->prev;
}

int cc_list_iter_init(struct cc_list_iter *self, struct cc_list *list, uint8_t direction) {
	if (list == NULL)
		return 0;

	self->iterator = (struct cc_iter_i *)&iterator_interface;
	self->data = list;
	self->direction = direction;
	self->cursor = &self->data->root;
	cc_list_iter_step(self);

	return 1;
}

static int cc_list_iter_next(struct cc_list_iter *self, uintptr_t *value) {
	if (self->cursor == &self->data->root)
		return 0;

	*value = (uintptr_t)self->cursor->data;
	cc_list_iter_step(self);

	return 1;
}
