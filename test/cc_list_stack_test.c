#include "cc_list_stack.h"
#include <assert.h>

/// Example of using `cc_list` as a stack.

int main(void)
{
	struct cc_list_stack *list;
	uintptr_t tmp;

	assert(!cc_list_stack_new(&list, NULL));

	assert(cc_list_remove(list, 0, (void **)&tmp));

	assert(!cc_list_insert(list, 0, (void *)1));
	assert(!cc_list_remove(list, 0, (void **)&tmp));
	assert(tmp == 1);

	assert(!cc_list_insert(list, 0, (void *)1));
	assert(!cc_list_insert(list, 0, (void *)2));
	assert(!cc_list_insert(list, 0, (void *)3));

	assert(!cc_list_remove(list, 0, (void **)&tmp));
	assert(tmp == 3);
	assert(!cc_list_remove(list, 0, (void **)&tmp));
	assert(tmp == 2);
	assert(!cc_list_remove(list, 0, (void **)&tmp));
	assert(tmp == 1);

	assert(cc_list_remove(list, 0, (void **)&tmp));

	assert(!cc_list_stack_delete(list));
	return 0;
}
