#include "cc_list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/// Example of using `cc_list` as a queue.

int main() {
	struct cc_list *list;
	uintptr_t tmp;

	list = cc_list_new();
	assert(list != NULL);

	assert(cc_list_remove(list, 0, (void **)&tmp));

	assert(!cc_list_append(list, (void *)1));
	assert(!cc_list_append(list, (void *)2));
	assert(!cc_list_append(list, (void *)3));

	assert(!cc_list_remove(list, 0, (void **)&tmp));
	assert(tmp == 1);
	assert(!cc_list_remove(list, 0, (void **)&tmp));
	assert(tmp == 2);
	assert(!cc_list_remove(list, 0, (void **)&tmp));
	assert(tmp == 3);

	assert(cc_list_remove(list, 0, (void **)&tmp));

	assert(!cc_list_delete(list));
	return 0;
}
