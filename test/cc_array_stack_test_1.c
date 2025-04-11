#include "cc_array_stack.h"
#include <assert.h>

int main(void)
{
	struct cc_array_stack *stack;
	char i, tmp;

	assert(!cc_array_stack_new(&stack, 8, sizeof(char)));
	assert(cc_array_stack_space(stack) == 8);
	assert(cc_array_stack_elem_nums(stack) == 0);

	assert(cc_array_stack_pop(stack, &tmp) == CC_STACK_EMPTY);
	assert(cc_stack_pop(stack, (void **)&tmp) == CC_STACK_EMPTY);

	for (i = 0; i < 8; i++) {
		/*
		assert(!cc_array_stack_push(stack, &i));
		*/
		assert(!cc_stack_push(stack, &i));
		assert(cc_array_stack_elem_nums(stack) == (size_t)(i + 1));
		assert(cc_array_stack_space(stack) == (size_t)(8 - i - 1));
	}

	assert(cc_array_stack_space(stack) == 0);
	assert(cc_array_stack_push(stack, &i) == CC_STACK_FULL);
	assert(cc_stack_push(stack, &i) == CC_STACK_FULL);

	assert(!cc_array_stack_peek(stack, &i));
	assert(i == 7);
	assert(!cc_array_stack_peek(stack, &i));
	assert(i == 7);
	assert(!cc_stack_peek(stack, (void **)&i));
	assert(i == 7);

	for (i = 0; i < 8; i++) {
		/*
		assert(!cc_array_stack_pop(stack, &tmp));
		*/
		assert(!cc_stack_pop(stack, (void **)&tmp));
		assert(tmp == 8 - i - 1);
		assert(cc_array_stack_elem_nums(stack) == (size_t)(8 - i - 1));
		assert(cc_array_stack_space(stack) == (size_t)(i + 1));
	}

	assert(cc_array_stack_elem_nums(stack) == 0);
	assert(cc_array_stack_space(stack) == 8);
	assert(cc_array_stack_pop(stack, &tmp) == CC_STACK_EMPTY);
	assert(cc_stack_pop(stack, (void **)&tmp) == CC_STACK_EMPTY);

	assert(!cc_array_stack_delete(stack));

	return 0;
}
