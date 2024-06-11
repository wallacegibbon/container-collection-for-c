#ifndef __CC_LIST_H
#define __CC_LIST_H

#ifdef NO_MALLOC
#error "You can NOT use `cc_list` without support for the `malloc` function."
#endif

#include "cc_iter.h"

/// The `cc_list_node` is used both for real nodes and the root node.
/// For root node, the `data` field is used as `size` to keep the number of elements in the list.
struct cc_list_node {
	struct cc_list_node *prev;
	struct cc_list_node *next;
	union {
		void *data;
		size_t size;
	};
};

struct cc_list {
	struct cc_list_node root;
};

int cc_list_new(struct cc_list **self);
int cc_list_delete(struct cc_list *self);

int cc_list_append(struct cc_list *self, void *value);
int cc_list_concat(struct cc_list *left, struct cc_list *right);
int cc_list_insert(struct cc_list *self, size_t index, void *value);
int cc_list_remove(struct cc_list *self, size_t index, void **result);

int cc_list_print(struct cc_list *self, int direction);

struct cc_list_iter {
	struct cc_iter_i *iterator;
	struct cc_list *list;
	struct cc_list_node *cursor;
	size_t index;
	int direction;
};

int cc_list_iter_init(struct cc_list_iter *self, struct cc_list *list, int direction);
int cc_list_iter_next(struct cc_list_iter *self, void **item, size_t *index);

#endif
