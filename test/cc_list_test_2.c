#include "cc_list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/// Example of using `cc_list` as a stack.

int main() {
	struct cc_list *list;
	size_t tmp;

	list = cc_list_new();
	if (list == NULL)
		return 1;

	assert(!cc_list_remove(list, 0, (void **)&tmp));

	assert(cc_list_insert(list, 0, (void *)1));
	assert(cc_list_remove(list, 0, (void **)&tmp));
	assert(tmp == 1);

	assert(cc_list_insert(list, 0, (void *)1));
	assert(cc_list_insert(list, 0, (void *)2));
	assert(cc_list_insert(list, 0, (void *)3));

	assert(cc_list_remove(list, 0, (void **)&tmp));
	assert(tmp == 3);
	assert(cc_list_remove(list, 0, (void **)&tmp));
	assert(tmp == 2);
	assert(cc_list_remove(list, 0, (void **)&tmp));
	assert(tmp == 1);

	assert(!cc_list_remove(list, 0, (void **)&tmp));

	cc_list_delete(list, NULL);
	return 0;
}
