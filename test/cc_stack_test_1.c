#include "cc_stack.h"
#include <stdio.h>

int main() {
	struct cc_array array;
	struct cc_stack stack, *p_stack;
	uint8_t buffer[10];
	char i;
	char tmp;

	p_stack = cc_stack_new(10, sizeof(char));
	if (p_stack == NULL)
		return 1;

	// cc_array_init(&array, buffer, 10, 1);
	// cc_stack_init(&stack, &array);
	// p_stack = &stack;

	for (i = 0; i < 10; i++)
		check_ret(cc_stack_push(p_stack, &i));

	for (i = 9; i >= 0; i--) {
		check_ret(cc_stack_pop(p_stack, &tmp));
		check_ret(tmp == i);
	}

	cc_stack_delete(p_stack);

	return 0;
}
