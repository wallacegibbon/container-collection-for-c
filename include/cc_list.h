#ifndef __CC_LIST_H
#define __CC_LIST_H

#include "cc_array.h"
#include "cc_common.h"
#include "cc_iter.h"

/******************************************************************************
	The List Node
 *****************************************************************************/
struct cc_list_node {
	struct cc_list_node *prev;
	struct cc_list_node *next;
	/*
	 * For data node, we use `data` to hold a value or a pointer to value.
	 * For root node, we use `size` to keep the number of elements.
	 */
	union {
		void *data;
		size_t size;
	};
};

int cc_list_node_insert_before(struct cc_list_node *self, void *data);
int cc_list_node_insert_after(struct cc_list_node *self, void *data);
int cc_list_node_remove_before(struct cc_list_node *self, void **result);
int cc_list_node_remove_after(struct cc_list_node *self, void **result);

int cc_list_node_delete_and_next(struct cc_list_node **current,
		cc_delete_fn_t remove_fn);

/******************************************************************************
	The List Container
 *****************************************************************************/
enum cc_list_error {
	CC_LIST_EMPTY = 0xFE10,
};

struct cc_list {
	struct cc_list_node root;
};

int cc_list_new(struct cc_list **self);
int cc_list_delete(struct cc_list *self);

int cc_list_concat(struct cc_list *left, struct cc_list *right);

int cc_list_insert_head(struct cc_list *self, void *data);
int cc_list_insert_tail(struct cc_list *self, void *data);
int cc_list_remove_head(struct cc_list *self, void **data);
int cc_list_remove_tail(struct cc_list *self, void **data);
int cc_list_get_head(struct cc_list *self, void **data);
int cc_list_get_tail(struct cc_list *self, void **data);

int cc_list_is_empty(struct cc_list *self);
size_t cc_list_size(struct cc_list *self);

int cc_list_to_cc_array(struct cc_list *self, struct cc_array **result);

/* For debugging */
int cc_list_print(struct cc_list *self, int direction);

/******************************************************************************
	The List Cursor
 *****************************************************************************/
enum cc_list_cursor_error {
	CC_LIST_CURSOR_MOVE_OUT_OF_RANGE = 0xFF10,
	CC_LIST_CURSOR_GET_OUT_OF_RANGE = 0xFF11,
	CC_LIST_CURSOR_REMOVING_CURRENT = 0xFF19,
	CC_LIST_CURSOR_AT_END = 0xFF90,
};

struct cc_list_cursor {
	struct cc_list *list;
	struct cc_list_node *current;
	cc_delete_fn_t remove_fn;
};

int cc_list_cursor_init(struct cc_list_cursor *tmp, struct cc_list *list,
		cc_delete_fn_t remove_fn);

int cc_list_cursor_new(struct cc_list_cursor **self, struct cc_list *list,
		cc_delete_fn_t remove_fn);

int cc_list_cursor_delete(struct cc_list_cursor *self);

int cc_list_cursor_relative_pos(struct cc_list_cursor *self, int offset,
		struct cc_list_node **result);

int cc_list_cursor_get(struct cc_list_cursor *self, int offset, int count,
		void **result);

int cc_list_cursor_move(struct cc_list_cursor *self, int offset);

int cc_list_cursor_insert_before(struct cc_list_cursor *self, int offset,
		void *data);

int cc_list_cursor_remove(struct cc_list_cursor *self, int offset, int count);

int cc_list_cursor_at_end(struct cc_list_cursor *self);
void cc_list_cursor_reset(struct cc_list_cursor *self);

/******************************************************************************
	The List Iterator
 *****************************************************************************/
struct cc_list_iter {
	struct cc_iter_i *iterator;
	struct cc_list *list;
	struct cc_list_node *cursor;
	size_t index;
	int direction;
};

int cc_list_iter_init(struct cc_list_iter *self, struct cc_list *list,
		int direction);

int cc_list_iter_next(struct cc_list_iter *self, void **item, size_t *index);

#endif
