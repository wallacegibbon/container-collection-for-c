// #include "cc_common.h"
#include "cc_list.h"
#include <assert.h>
#include <stdint.h>

int main(void)
{
	struct cc_list *list;
	struct cc_list_cursor *cursor;
	uintptr_t buffer[16] = {0};
	uintptr_t i;

	assert(!cc_list_new(&list));

	for (i = 0; i < 16; i++)
		assert(!cc_list_insert_tail(list, (void *)i));

	assert(!cc_list_cursor_new(&cursor, list, NULL));

	/// cursor is at end on start
	assert(cc_list_cursor_get(cursor, 0, 1, (void **)buffer) == CC_LIST_CURSOR_GET_OUT_OF_RANGE);

	/// cursor is on the first element after reset
	cc_list_cursor_reset(cursor);
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

	/// The list has only 14 elements from the `current` pointer now. (16 elements in total)
	// cc_debug_print("after move: list size: %d\n", cc_list_size(cursor->list));
	assert(cc_list_size(cursor->list) == 16);

	assert(cc_list_cursor_remove(cursor, 14, 1) == CC_LIST_CURSOR_MOVE_OUT_OF_RANGE);
	assert(cc_list_cursor_remove(cursor, 13, 2) == CC_LIST_CURSOR_MOVE_OUT_OF_RANGE);
	assert(!cc_list_cursor_remove(cursor, 13, 1));

	/// The list has only 13 elements from the `current` pointer now. (15 elements in total)
	// cc_debug_print("after 1st remove: list size: %d\n", cc_list_size(cursor->list));
	assert(cc_list_size(cursor->list) == 15);

	assert(cc_list_cursor_insert_before(cursor, 16, (void *)99) == CC_LIST_CURSOR_MOVE_OUT_OF_RANGE);
	assert(cc_list_cursor_insert_before(cursor, 15, (void *)99) == CC_LIST_CURSOR_MOVE_OUT_OF_RANGE);
	assert(cc_list_cursor_insert_before(cursor, 14, (void *)99) == CC_LIST_CURSOR_MOVE_OUT_OF_RANGE);
	assert(!cc_list_cursor_insert_before(cursor, 13, (void *)99));
	assert(cc_list_cursor_get(cursor, 12, 3, (void **)buffer) == CC_LIST_CURSOR_GET_OUT_OF_RANGE);
	assert(!cc_list_cursor_get(cursor, 12, 2, (void **)buffer));
	assert(buffer[0] == 14);
	assert(buffer[1] == 99);

	/// The list has only 14 elements from the `current` pointer now. (16 elements in total)
	// cc_debug_print("after insert (12, 99): list size: %d\n", cc_list_size(cursor->list));
	assert(cc_list_size(cursor->list) == 16);

	assert(cc_list_cursor_remove(cursor, -1, 2) == CC_LIST_CURSOR_REMOVING_CURRENT);
	assert(cc_list_cursor_remove(cursor, -3, 2) == CC_LIST_CURSOR_MOVE_OUT_OF_RANGE);
	/// Before removing, `-2` is a valid range start
	assert(!cc_list_cursor_get(cursor, -2, 3, (void **)buffer));
	assert(!cc_list_cursor_remove(cursor, -1, 1));
	/// After removing, `-2` is a NOT valid range start
	assert(cc_list_cursor_get(cursor, -2, 3, (void **)buffer) == CC_LIST_CURSOR_MOVE_OUT_OF_RANGE);

	/// The list has only 14 elements from the `current` pointer now. (15 elements in total)
	// cc_debug_print("after remove (-1, 1): list size: %d\n", cc_list_size(cursor->list));
	assert(cc_list_size(cursor->list) == 15);

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

	/// The list has only 12 elements after the `current` pointer now. (13 elements in total)
	// cc_debug_print("after remove (1, 2): list size: %d\n", cc_list_size(cursor->list));
	assert(cc_list_size(cursor->list) == 13);

	assert(!cc_list_cursor_get(cursor, -1, 3, (void **)buffer));
	assert(!cc_list_cursor_insert_before(cursor, 1, (void *)99));
	assert(!cc_list_cursor_get(cursor, -1, 4, (void **)buffer));
	assert(buffer[0] == 0);
	assert(buffer[1] == 2);
	assert(buffer[2] == 99);
	assert(buffer[3] == 5);

	/// The list has only 13 elements after the `current` pointer now. (14 elements in total)
	// cc_debug_print("after insert (0, 99): list size: %d\n", cc_list_size(cursor->list));
	assert(cc_list_size(cursor->list) == 14);

	assert(cc_list_cursor_get(cursor, -1, 15, (void **)buffer) == CC_LIST_CURSOR_GET_OUT_OF_RANGE);
	assert(!cc_list_cursor_get(cursor, -1, 14, (void **)buffer));
	assert(buffer[0] == 0);
	assert(buffer[1] == 2);
	assert(buffer[12] == 14);
	assert(buffer[13] == 99);

	assert(!cc_list_cursor_at_end(cursor));

	assert(!cc_list_cursor_remove(cursor, 1, 12));
	assert(cc_list_size(cursor->list) == 2);
	assert(!cc_list_cursor_remove(cursor, -1, 1));
	assert(cc_list_size(cursor->list) == 1);

	/// removing will fail here since curor in on this element
	assert(cc_list_cursor_remove(cursor, 0, 1) == CC_LIST_CURSOR_REMOVING_CURRENT);
	assert(cc_list_size(cursor->list) == 1);

	assert(!cc_list_cursor_at_end(cursor));

	/// remove after move is OK.
	assert(!cc_list_cursor_move(cursor, 1));
	assert(!cc_list_cursor_remove(cursor, -1, 1));
	assert(cc_list_size(cursor->list) == 0);

	assert(cc_list_cursor_at_end(cursor));

	assert(!cc_list_cursor_delete(cursor));
	assert(!cc_list_delete(list));

	return 0;
}
