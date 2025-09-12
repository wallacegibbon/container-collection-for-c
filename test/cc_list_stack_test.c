#include "cc_list_stack.h"
#include <assert.h>
#include <stdint.h>

int
main(void)
{
	cc_ListStack *list;
	uintptr_t tmp;

	assert(!cc_list_stack_new(&list));

	assert(cc_stack_pop(list, (void **)&tmp) == CC_STACK_EMPTY);

	assert(!cc_stack_push(list, (void *)1));
	assert(!cc_stack_peek(list, (void **)&tmp));
	assert(tmp == 1);

	assert(!cc_stack_push(list, (void *)2));
	assert(!cc_stack_push(list, (void *)3));

	assert(!cc_stack_pop(list, (void **)&tmp));
	assert(tmp == 3);
	assert(!cc_stack_pop(list, (void **)&tmp));
	assert(tmp == 2);
	assert(!cc_stack_pop(list, (void **)&tmp));
	assert(tmp == 1);

	assert(cc_stack_pop(list, (void **)&tmp) == CC_STACK_EMPTY);

	assert(!cc_list_stack_delete(list));
	return 0;
}
