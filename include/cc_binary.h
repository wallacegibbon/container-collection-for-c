#ifndef __CC_BINARY_H
#define __CC_BINARY_H

#include "cc_common.h"
#include "cc_iter.h"
#include "cc_list.h"
#include <stddef.h>

typedef struct cc_Binary cc_Binary;

struct cc_Binary {
	cc_Binary *parent;
	cc_Binary *left;
	cc_Binary *right;
	void *data;
};

int	cc_binary_new(cc_Binary **self, cc_Binary *parent, void *data);
int	cc_binary_delete(cc_Binary *self);
int	cc_binary_insert_left(cc_Binary *self, void *data);
int	cc_binary_insert_right(cc_Binary *self, void *data);
int	cc_binary_rotate_left(cc_Binary **start_slot);
int	cc_binary_rotate_right(cc_Binary **start_slot);
int	cc_binary_print(cc_Binary *root, int depth, cc_simple_fn_1_t print_fn);

typedef struct cc_BinaryIter cc_BinaryIter;

struct cc_BinaryIter {
	cc_IterI *iterator;
	cc_Binary *tree;
	cc_List *queue;
	size_t index;
	cc_TraverseDirection direction;
};

/*
This iterator do not provide `_init` method since it need a `_delete` to free
some resources.
*/
int	cc_binary_iter_new(cc_BinaryIter **self, cc_Binary *root,
		cc_TraverseDirection direction);

int	cc_binary_iter_delete(cc_BinaryIter *self);

int	cc_binary_iter_next(cc_BinaryIter *self, void **item, size_t *index);

#endif
