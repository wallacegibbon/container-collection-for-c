#include "cc_common.h"
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

	assert(!cc_list_cursor_new(&cursor, list));

	assert(!cc_list_cursor_get_n(cursor, 0, 3, (void **)buffer));
	assert(buffer[0] == 0);
	assert(buffer[1] == 1);
	assert(buffer[2] == 2);

	assert(!cc_list_cursor_get_n(cursor, 2, 3, (void **)buffer));
	assert(buffer[0] == 2);
	assert(buffer[1] == 3);
	assert(buffer[2] == 4);

	assert(!cc_list_cursor_move_n(cursor, 2));

	assert(!cc_list_cursor_get_n(cursor, -2, 3, (void **)buffer));
	assert(buffer[0] == 0);
	assert(buffer[1] == 1);
	assert(buffer[2] == 2);

	assert(!cc_list_cursor_get_n(cursor, 0, 3, (void **)buffer));
	assert(buffer[0] == 2);
	assert(buffer[1] == 3);
	assert(buffer[2] == 4);

	assert(!cc_list_cursor_delete(cursor));
	assert(!cc_list_delete(list));

	return 0;
}
