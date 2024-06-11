#ifndef __CC_BINARY_H
#define __CC_BINARY_H

#ifdef NO_MALLOC
#error "You can NOT use `cc_binary` without support for the `malloc` function."
#endif

#include "cc_common.h"
#include "cc_iter.h"
#include <stddef.h>

struct cc_binary {
	struct cc_binary *parent;
	struct cc_binary *left;
	struct cc_binary *right;
	void *data;
};

int cc_binary_new(struct cc_binary **self, struct cc_binary *parent, void *data);
int cc_binary_delete(struct cc_binary *self);

int cc_binary_insert_left(struct cc_binary *self, void *data);
int cc_binary_insert_right(struct cc_binary *self, void *data);

int cc_binary_rotate_left(struct cc_binary **start_slot);
int cc_binary_rotate_right(struct cc_binary **start_slot);

int cc_binary_print(struct cc_binary *root, int depth, cc_simple_fn_1_t print_fn);

struct cc_binary_iter {
	struct cc_iter_i *iterator;
	struct cc_binary *tree;
	struct cc_list *queue;
	size_t index;
	enum cc_traverse_direction direction;
};

/// This iterator do not provide `_init` method since it need a `_delete` to free some resources.
int cc_binary_iter_new(struct cc_binary_iter **self, struct cc_binary *root, enum cc_traverse_direction direction);
int cc_binary_iter_delete(struct cc_binary_iter *self);

int cc_binary_iter_next(struct cc_binary_iter *self, void **item, size_t *index);

#endif
