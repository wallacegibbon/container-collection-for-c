#include "cc_list.h"
#include "cc_common.h"
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
/// List Cursor Functions
////////////////////////////////////////////////////////////////////////////////
int cc_list_cursor_relative_next(struct cc_list_cursor *self, int offset, struct cc_list_node **result)
{
	struct cc_list_node *node;
	/// The `last` element can be the root node, so it's `n != &self->list->root` here
	for (node = self->current; node != &self->list->root && offset > 0; offset--)
		node = node->next;

	if (offset > 0)
		return CC_LIST_CURSOR_MOVE_OUT_OF_RANGE;

	*result = node;
	return 0;
}

int cc_list_cursor_relative_prev(struct cc_list_cursor *self, int offset, struct cc_list_node **result)
{
	struct cc_list_node *node;
	/// The `first` element can NOT be the root node, so it's `n->prev != &self->list->root` here
	for (node = self->current; node->prev != &self->list->root && offset > 0; offset--)
		node = node->prev;

	if (offset > 0)
		return CC_LIST_CURSOR_MOVE_OUT_OF_RANGE;

	*result = node;
	return 0;
}

/// This function can only return 0 or CC_LIST_CURSOR_MOVE_OUT_OF_RANGE
int cc_list_cursor_relative_pos(struct cc_list_cursor *self, int offset, struct cc_list_node **result)
{
	if (offset >= 0)
		return cc_list_cursor_relative_next(self, offset, result);
	else
		return cc_list_cursor_relative_prev(self, -offset, result);
}

int cc_list_cursor_insert_after(struct cc_list_cursor *self, int offset, void *data)
{
	struct cc_list_node *node;

	if (cc_list_cursor_relative_pos(self, offset, &node))
		return CC_LIST_CURSOR_MOVE_OUT_OF_RANGE;

	/// Calling `_next` to reach the root node is valid (so that you can use ranges),
	/// but inserting is invalid in this case.
	if (node == &self->list->root)
		return CC_LIST_CURSOR_INSERT_OUT_OF_RANGE;

	if (cc_list_node_insert_after(node, data))
		return 1;

	return 0;
}

int cc_list_cursor_remove(struct cc_list_cursor *self, int offset, int count)
{
	struct cc_list_node *node1, *node2;

	if (offset <= 0 && offset + count > 0)
		return CC_LIST_CURSOR_REMOVING_CURRENT;
	if (cc_list_cursor_relative_pos(self, offset, &node1))
		return CC_LIST_CURSOR_MOVE_OUT_OF_RANGE;
	if (cc_list_cursor_relative_pos(self, offset + count, &node2))
		return CC_LIST_CURSOR_MOVE_OUT_OF_RANGE;

	/// fix the chain before removing nodes
	node1->prev->next = node2;
	node2->prev = node1->prev;

	while (node1 != node2) {
		if (cc_list_node_delete_and_next(&node1, self->remove_fn))
			return 5;
	}

	return 0;
}

/// Caution: Please make sure that `result` can hold `count` numbers of pointers.
int cc_list_cursor_get(struct cc_list_cursor *self, int offset, int count, void **result)
{
	struct cc_list_node *node;
	int i;

	if (cc_list_cursor_relative_pos(self, offset, &node))
		return CC_LIST_CURSOR_MOVE_OUT_OF_RANGE;

	/// Calling `_next` to reach the root node is valid (so that you can use ranges),
	/// but getting is invalid in this case.
	if (node == &self->list->root)
		return CC_LIST_CURSOR_GET_OUT_OF_RANGE;

	for (i = 0; i < count && node != &self->list->root; i++, node = node->next)
		result[i] = node->data;

	if (i < count)
		return CC_LIST_CURSOR_GET_OUT_OF_RANGE;

	return 0;
}

int cc_list_cursor_move(struct cc_list_cursor *self, int offset)
{
	struct cc_list_node *new_pos;

	if (cc_list_cursor_relative_pos(self, offset, &new_pos))
		return CC_LIST_CURSOR_MOVE_OUT_OF_RANGE;

	self->current = new_pos;
	return 0;
}

int cc_list_cursor_init(struct cc_list_cursor *tmp, struct cc_list *list, cc_list_node_data_remove_fn_t remove_fn)
{
	tmp->remove_fn = remove_fn;
	tmp->list = list;
	if (list->root.next == &list->root)
		tmp->current = NULL;
	else
		tmp->current = list->root.next;

	return 0;
}

int cc_list_cursor_new(struct cc_list_cursor **self, struct cc_list *list, cc_list_node_data_remove_fn_t remove_fn)
{
	struct cc_list_cursor *tmp;

	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		goto fail1;
	if (cc_list_cursor_init(tmp, list, remove_fn))
		goto fail2;

	*self = tmp;
	return 0;
fail2:
	free(tmp);
fail1:
	return 1;
}

int cc_list_cursor_delete(struct cc_list_cursor *self)
{
	free(self);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// List Iterator Functions
////////////////////////////////////////////////////////////////////////////////
static struct cc_iter_i iterator_interface = {
	.next = (cc_iter_next_fn_t)cc_list_iter_next,
};

static inline void cc_list_iter_step(struct cc_list_iter *self)
{
	if (self->direction == 0)
		self->cursor = self->cursor->next;
	else
		self->cursor = self->cursor->prev;
}

int cc_list_iter_next(struct cc_list_iter *self, void **item, size_t *index)
{
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

int cc_list_iter_init(struct cc_list_iter *self, struct cc_list *list, int direction)
{
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

////////////////////////////////////////////////////////////////////////////////
/// List Node Functions
////////////////////////////////////////////////////////////////////////////////
int cc_list_node_insert_after(struct cc_list_node *self, void *data)
{
	struct cc_list_node *node;

	node = malloc(sizeof(*node));
	if (node == NULL)
		return 1;

	node->data = data;
	node->next = self->next;
	node->prev = self;
	self->next->prev = node;
	self->next = node;
	return 0;
}

int cc_list_node_remove_after(struct cc_list_node *self, void **result)
{
	struct cc_list_node *node;

	/// You have to provide `result`, or the `self->next->data` may leak.
	if (try_reset_double_p(result))
		return 1;

	*result = self->next->data;
	node = self->next;
	self->next->next->prev = self;
	self->next = self->next->next;

	free(node);
	return 0;
}

int cc_list_node_delete_and_next(struct cc_list_node **pcurrent, cc_list_node_data_remove_fn_t remove_fn)
{
	struct cc_list_node *current, *next;
	current = *pcurrent;
	next = current->next;
	if (remove_fn != NULL) {
		if (remove_fn(current->data))
			return 1;
	}

	free(current);

	*pcurrent = next;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// List Functions
////////////////////////////////////////////////////////////////////////////////
static int prev_node_of(struct cc_list *self, struct cc_list_node **result, size_t index)
{
	struct cc_list_node *node;

	node = &self->root;
	while (index--)
		node = node->next;

	*result = node;
	return 0;
}

/// Range of `index`: 0 ~ self->root.size
int cc_list_insert(struct cc_list *self, size_t index, void *data)
{
	struct cc_list_node *entry, *node;

	if (index > self->root.size)
		return 1;
	if (prev_node_of(self, &entry, index))
		return 2;
	if (cc_list_node_insert_after(entry, data))
		return 3;

	self->root.size++;
	return 0;
}

/// Range of `index`: 0 ~ (self->root.size - 1)
int cc_list_remove(struct cc_list *self, size_t index, void **result)
{
	struct cc_list_node *node, *node_to_remove;

	if (index >= self->root.size)
		return 1;
	if (prev_node_of(self, &node, index))
		return 2;
	if (cc_list_node_remove_after(node, result))
		return 3;

	self->root.size--;
	return 0;
}

size_t cc_list_size(struct cc_list *self)
{
	return self->root.size;
}

int cc_list_append(struct cc_list *self, void *data)
{
	struct cc_list_node *node;

	node = malloc(sizeof(*node));
	if (node == NULL)
		return 1;

	node->data = data;
	self->root.prev->next = node;
	node->prev = self->root.prev;

	self->root.prev = node;
	node->next = &self->root;

	self->root.size++;
	return 0;
}

int cc_list_init(struct cc_list *self)
{
	self->root.prev = &self->root;
	self->root.next = &self->root;
	self->root.size = 0;
	return 0;
}

/// Caution: You may need to delete `right` list after this concatenation.
int cc_list_concat(struct cc_list *left, struct cc_list *right)
{
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

int cc_list_new(struct cc_list **self)
{
	struct cc_list *tmp;

	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		goto fail1;
	if (cc_list_init(tmp))
		goto fail2;

	*self = tmp;
	return 0;

fail2:
	free(tmp);
fail1:
	return 1;
}

int cc_list_delete(struct cc_list *self)
{
	struct cc_list_node *node;

	node = self->root.next;
	while (node != &self->root) {
		if (cc_list_node_delete_and_next(&node, NULL))
			return 1;
	}

	free(self);
	return 0;
}

int cc_list_print(struct cc_list *self, int direction)
{
	struct cc_list_iter iter;
	size_t *tmp;
	size_t index;

	cc_debug_print("list content: \n");
	if (cc_list_iter_init(&iter, self, direction))
		return 1;
	while (!cc_iter_next(&iter, &tmp, &index))
		cc_debug_print("(%d)%llu, ", index, *tmp);

	cc_debug_print("\n\n");
	return 0;
}
