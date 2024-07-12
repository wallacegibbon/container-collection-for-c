#include "cc_stack.h"
#include <assert.h>

int main(void)
{
	struct cc_array array;
	struct cc_stack stack, *p_stack;
	// unsigned char buffer[10];
	char i, tmp;

	assert(!cc_stack_new(&p_stack, 10, sizeof(char)));

	// assert(!cc_array_init(&array, buffer, 10, 1));
	// assert(!cc_stack_init(&stack, &array));
	// p_stack = &stack;

	for (i = 0; i < 10; i++)
		assert(!cc_stack_push(p_stack, &i));

	assert(!cc_stack_peek(p_stack, &i));
	assert(i == 9);
	assert(!cc_stack_peek(p_stack, &i));
	assert(i == 9);

	for (i = 9; i >= 0; i--) {
		assert(!cc_stack_pop(p_stack, &tmp));
		assert(tmp == i);
	}

	assert(!cc_stack_delete(p_stack));

	return 0;
}
