#include "cc_list.h"
#include <assert.h>
#include <stdint.h>

int main(void)
{
	struct cc_list *list;
	struct cc_list_cursor *cursor;
	uintptr_t buffer[4] = {0};
	uintptr_t i;

	assert(!cc_list_new(&list));

	for (i = 0; i < 16; i++)
		assert(!cc_list_append(list, (void *)i));

	assert(!cc_list_cursor_new(&cursor, list, NULL));

	assert(!cc_list_cursor_get(cursor, 0, 1, (void **)buffer));

	assert(cc_list_cursor_get(cursor, -1, 1, (void **)buffer) == CC_LIST_CURSOR_MOVE_OUT_OF_RANGE);
	assert(cc_list_cursor_get(cursor, 16, 1, (void **)buffer) == CC_LIST_CURSOR_GET_OUT_OF_RANGE);

	assert(!cc_list_cursor_get(cursor, 15, 1, (void **)buffer));
	assert(cc_list_cursor_get(cursor, 15, 2, (void **)buffer) == CC_LIST_CURSOR_GET_OUT_OF_RANGE);

	assert(!cc_list_cursor_get(cursor, 0, 3, (void **)buffer));
	assert(buffer[0] == 0);
	assert(buffer[1] == 1);
	assert(buffer[2] == 2);

	assert(!cc_list_cursor_get(cursor, 2, 3, (void **)buffer));
	assert(buffer[0] == 2);
	assert(buffer[1] == 3);
	assert(buffer[2] == 4);

	assert(!cc_list_cursor_move(cursor, 2));
	assert(!cc_list_cursor_get(cursor, -1, 1, (void **)buffer));
	assert(!cc_list_cursor_get(cursor, -2, 1, (void **)buffer));
	assert(cc_list_cursor_get(cursor, -3, 1, (void **)buffer) == CC_LIST_CURSOR_MOVE_OUT_OF_RANGE);

	assert(!cc_list_cursor_get(cursor, -2, 3, (void **)buffer));
	assert(buffer[0] == 0);
	assert(buffer[1] == 1);
	assert(buffer[2] == 2);

	assert(!cc_list_cursor_get(cursor, 0, 3, (void **)buffer));
	assert(buffer[0] == 2);
	assert(buffer[1] == 3);
	assert(buffer[2] == 4);

	/// The list has only 13 elements after the `current` pointer now.
	assert(cc_list_cursor_remove(cursor, 14, 1) == CC_LIST_CURSOR_MOVE_OUT_OF_RANGE);
	assert(cc_list_cursor_remove(cursor, 13, 2) == CC_LIST_CURSOR_MOVE_OUT_OF_RANGE);
	assert(!cc_list_cursor_remove(cursor, 13, 1));
	/// The list has only 12 elements after the `current` pointer now.

	assert(cc_list_cursor_insert_after(cursor, 15, (void *)99) == CC_LIST_CURSOR_MOVE_OUT_OF_RANGE);
	assert(cc_list_cursor_insert_after(cursor, 14, (void *)99) == CC_LIST_CURSOR_MOVE_OUT_OF_RANGE);
	assert(cc_list_cursor_insert_after(cursor, 13, (void *)99) == CC_LIST_CURSOR_INSERT_OUT_OF_RANGE);
	assert(!cc_list_cursor_insert_after(cursor, 12, (void *)99));
	assert(cc_list_cursor_get(cursor, 12, 3, (void **)buffer) == CC_LIST_CURSOR_GET_OUT_OF_RANGE);
	assert(!cc_list_cursor_get(cursor, 12, 2, (void **)buffer));
	assert(buffer[0] == 14);
	assert(buffer[1] == 99);

	assert(cc_list_cursor_remove(cursor, -1, 2) == CC_LIST_CURSOR_REMOVING_CURRENT);
	assert(cc_list_cursor_remove(cursor, -3, 2) == CC_LIST_CURSOR_MOVE_OUT_OF_RANGE);
	/// Before removing, `-2` is a valid range start
	assert(!cc_list_cursor_get(cursor, -2, 3, (void **)buffer));
	assert(!cc_list_cursor_remove(cursor, -1, 1));
	/// After removing, `-2` is a NOT valid range start
	assert(cc_list_cursor_get(cursor, -2, 3, (void **)buffer) == CC_LIST_CURSOR_MOVE_OUT_OF_RANGE);

	assert(!cc_list_cursor_get(cursor, -1, 3, (void **)buffer));
	assert(buffer[0] == 0);
	assert(buffer[1] == 2);
	assert(buffer[2] == 3);

	assert(cc_list_cursor_remove(cursor, 0, 2) == CC_LIST_CURSOR_REMOVING_CURRENT);
	assert(!cc_list_cursor_remove(cursor, 1, 2));
	assert(!cc_list_cursor_get(cursor, -1, 3, (void **)buffer));
	assert(buffer[0] == 0);
	assert(buffer[1] == 2);
	assert(buffer[2] == 5);

	assert(!cc_list_cursor_insert_after(cursor, 0, (void *)99));
	assert(!cc_list_cursor_get(cursor, -1, 4, (void **)buffer));
	assert(buffer[0] == 0);
	assert(buffer[1] == 2);
	assert(buffer[2] == 99);
	assert(buffer[3] == 5);

	assert(!cc_list_cursor_delete(cursor));
	assert(!cc_list_delete(list));

	return 0;
}
