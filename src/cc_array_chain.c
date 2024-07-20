#include "cc_array_chain.h"
#include "cc_array.h"
#include "cc_list.h"
#include <stdlib.h>
#include <string.h>

int cc_array_chain_node_new(struct cc_array_chain *self);

int cc_array_chain_add_elem(struct cc_array_chain *self, void *elem)
{
	if (self->cursor == NULL) {
		if (cc_array_chain_node_new(self))
			return 1;
	}

	if (cc_array_set(self->cursor, self->cursor_index++, elem) == CC_ARRAY_OUT_OF_RANGE) {
		if (cc_array_chain_node_new(self))
			return 2;
		if (cc_array_set(self->cursor, self->cursor_index++, elem))
			return 3;
	}

	self->total_nums++;
	return 0;
}

int cc_array_chain_append(struct cc_array_chain *self, void *data, size_t nums)
{
	size_t i;
	for (i = 0; i < nums; i++) {
		if (cc_array_chain_add_elem(self, (char *)data + i * self->node_elem_size))
			return 1;
	}
	return 0;
}

int cc_array_chain_node_size(struct cc_array_chain *self, size_t index)
{
	size_t n;
	n = self->total_nums - self->node_elem_nums * index;
	if (n > self->node_elem_nums)
		return self->node_elem_nums;
	else
		return n;
}

int cc_array_chain_to_array(struct cc_array_chain *self, struct cc_array **result, int nums_to_reserve)
{
	struct cc_list_iter iter;
	struct cc_array *r, **tmp;
	size_t i, size;

	if (cc_array_new(&r, self->total_nums + nums_to_reserve, self->node_elem_size))
		goto fail1;
	if (cc_list_iter_init(&iter, self->node_chain, 0))
		goto fail2;
	while (!cc_iter_next(&iter, &tmp, &i))
		memcpy(r->data + self->node_elem_nums * i, (*tmp)->data, cc_array_chain_node_size(self, i));

	*result = r;
	return 0;

fail2:
	cc_array_delete(r);
fail1:
	return 1;
}

int cc_array_chain_new(struct cc_array_chain **self, int node_elem_nums, int node_elem_size)
{
	struct cc_array_chain *tmp;

	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		goto fail1;
	if (cc_list_new(&tmp->node_chain))
		goto fail2;

	tmp->node_elem_nums = node_elem_nums;
	tmp->node_elem_size = node_elem_size;

	tmp->cursor = NULL;
	tmp->cursor_index = 0;
	tmp->total_nums = 0;

	*self = tmp;
	return 0;

fail3:
	cc_list_delete(tmp->node_chain);
fail2:
	free(tmp);
fail1:
	return 1;
}

int cc_array_chain_delete(struct cc_array_chain *self)
{
	struct cc_list_iter iter;
	struct cc_array **tmp;

	if (cc_list_iter_init(&iter, self->node_chain, 0))
		return 1;
	while (!cc_iter_next(&iter, &tmp, NULL)) {
		if (cc_array_delete(*tmp))
			return 2;
	}

	if (cc_list_delete(self->node_chain))
		return 3;

	free(self);
	return 0;
}

int cc_array_chain_node_new(struct cc_array_chain *self)
{
	struct cc_array *arr;

	if (cc_array_new(&arr, self->node_elem_nums, self->node_elem_size))
		goto fail1;
	if (cc_list_append(self->node_chain, arr))
		goto fail2;

	self->cursor = arr;
	self->cursor_index = 0;
	return 0;

fail2:
	cc_array_delete(arr);
fail1:
	return 1;
}
