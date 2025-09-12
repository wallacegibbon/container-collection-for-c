#ifndef __CC_LIST_H
#define __CC_LIST_H

#include "cc_array.h"
#include "cc_common.h"
#include "cc_iter.h"

typedef struct cc_ListNode cc_ListNode;

struct cc_ListNode {
	cc_ListNode *prev;
	cc_ListNode *next;
	/*
	For data node, we use `data` to hold a value or a pointer to value.
	For root node, we use `size` to keep the number of elements.
	*/
	union {
		void *data;
		size_t size;
	};
};

int	cc_list_node_insert_before(cc_ListNode *self, void *data);
int	cc_list_node_insert_after(cc_ListNode *self, void *data);
int	cc_list_node_remove_before(cc_ListNode *self, void **result);
int	cc_list_node_remove_after(cc_ListNode *self, void **result);

int	cc_list_node_delete_and_next(cc_ListNode **current, cc_DelFn remove_fn);

typedef enum cc_ListError cc_ListError;

enum cc_ListError {
	CC_LIST_EMPTY = 0xFE10,
};

typedef struct cc_List cc_List;

struct cc_List {
	cc_ListNode root;
};

int	cc_list_new(cc_List **self);
int	cc_list_delete(cc_List *self);

int	cc_list_concat(cc_List *left, cc_List *right);

int	cc_list_insert_head(cc_List *self, void *data);
int	cc_list_insert_tail(cc_List *self, void *data);
int	cc_list_remove_head(cc_List *self, void **data);
int	cc_list_remove_tail(cc_List *self, void **data);
int	cc_list_get_head(cc_List *self, void **data);
int	cc_list_get_tail(cc_List *self, void **data);

int	cc_list_is_empty(cc_List *self);
size_t	cc_list_size(cc_List *self);

int	cc_list_to_cc_array(cc_List *self, cc_Array **result);

/* For debugging */
int	cc_list_print(cc_List *self, int direction);

typedef enum cc_ListCursorError cc_ListCursorError;

enum cc_ListCursorError {
	CC_LIST_CURSOR_MOVE_OUT_OF_RANGE = 0xFF10,
	CC_LIST_CURSOR_GET_OUT_OF_RANGE = 0xFF11,
	CC_LIST_CURSOR_REMOVING_CURRENT = 0xFF19,
	CC_LIST_CURSOR_AT_END = 0xFF90,
};

typedef struct cc_ListCursor cc_ListCursor;

struct cc_ListCursor {
	cc_List *list;
	cc_ListNode *current;
	cc_DelFn remove_fn;
};

int	cc_list_cursor_init(cc_ListCursor *tmp, cc_List *list, cc_DelFn remove_fn);
int	cc_list_cursor_new(cc_ListCursor **self, cc_List *list, cc_DelFn remove_fn);
int	cc_list_cursor_delete(cc_ListCursor *self);

int	cc_list_cursor_relative_pos(cc_ListCursor *self, int offset,
		cc_ListNode **result);

int	cc_list_cursor_get(cc_ListCursor *self, int offset, int count,
		void **result);

int	cc_list_cursor_move(cc_ListCursor *self, int offset);
int	cc_list_cursor_insert_before(cc_ListCursor *self, int offset,
		void *data);

int	cc_list_cursor_remove(cc_ListCursor *self, int offset, int count);

int	cc_list_cursor_at_end(cc_ListCursor *self);
void	cc_list_cursor_reset(cc_ListCursor *self);

typedef struct cc_ListIter cc_ListIter;

struct cc_ListIter {
	cc_IterI *iterator;
	cc_List *list;
	cc_ListNode *cursor;
	size_t index;
	int direction;
};

int	cc_list_iter_init(cc_ListIter *self, cc_List *list, int direction);
int	cc_list_iter_next(cc_ListIter *self, void **item, size_t *index);

#endif
