#ifndef __CC_ARRAY_CHAIN_H
#define __CC_ARRAY_CHAIN_H

#include "cc_array.h"
#include "cc_list.h"

typedef struct cc_ArrayChain cc_ArrayChain;

struct cc_ArrayChain {
	cc_List *node_chain;
	cc_Array *cursor;
	size_t cursor_index;
	size_t total_nums;
	size_t node_elem_nums;
	size_t node_elem_size;
};

int	cc_array_chain_new(cc_ArrayChain **self, int node_elem_nums,
		int node_elem_size);

int	cc_array_chain_delete(cc_ArrayChain *self);

int	cc_array_chain_add_elem(cc_ArrayChain *self, void *elem);
int	cc_array_chain_append(cc_ArrayChain *self, void *data, size_t nums);

int	cc_array_chain_to_array(cc_ArrayChain *self, cc_Array **result,
		int nums_to_reserve);

#endif
