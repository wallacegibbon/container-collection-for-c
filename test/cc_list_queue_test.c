#include "cc_list_queue.h"
#include <assert.h>

int main(void)
{
	struct cc_list_queue *list;
	uintptr_t tmp;

	assert(!cc_list_queue_new(&list, NULL));

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

	assert(!cc_list_queue_delete(list));
	return 0;
}
