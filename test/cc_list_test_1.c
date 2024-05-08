#include "cc_common.h"
#include "cc_list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	struct cc_list *list;
	uintptr_t i;

	list = cc_list_new();
	assert(list != NULL);

	for (i = 0; i < 10; i++) {
		assert(!cc_list_append(list, (void *)i));
		assert(list->root.size == i + 1);
	}

	assert(cc_list_insert(list, 11, (void *)55));
	assert(!cc_list_insert(list, 10, (void *)66));
	assert(!cc_list_insert(list, 0, (void *)77));
	assert(!cc_list_insert(list, 3, (void *)88));

	cc_list_print(list, 1);

	assert(cc_list_remove(list, 13, (void **)&i));
	assert(!cc_list_remove(list, 12, (void **)&i));
	assert(i == 66);
	assert(!cc_list_remove(list, 3, (void **)&i));
	assert(i == 88);
	assert(!cc_list_remove(list, 0, (void **)&i));
	assert(i == 77);

	cc_list_print(list, 1);

	assert(!cc_list_delete(list));
	return 0;
}
