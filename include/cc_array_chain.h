#ifndef __CC_ARRAY_CHAIN_H
#define __CC_ARRAY_CHAIN_H

#ifdef NO_MALLOC
#error "You can NOT use `cc_array_chain` without support for the `malloc` function."
#endif

#include "cc_array.h"
#include "cc_list.h"

struct cc_array_chain {
	struct cc_list *node_chain;
	struct cc_array *cursor;
	size_t cursor_index;
	size_t total_nums;
	size_t node_elem_nums;
	size_t node_elem_size;
};

int cc_array_chain_new(struct cc_array_chain **self, int node_elem_nums, int node_elem_size);
int cc_array_chain_delete(struct cc_array_chain *self);

int cc_array_chain_add_elem(struct cc_array_chain *self, void *elem);
int cc_array_chain_append(struct cc_array_chain *self, void *data, size_t nums);
int cc_array_chain_to_array(struct cc_array_chain *self, struct cc_array **result, int nums_to_reserve);

#endif
