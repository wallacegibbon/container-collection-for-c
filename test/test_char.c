#include "cc_array.h"
#include "cc_stack.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	struct cc_array array;
	struct cc_stack stack;
	uint8_t *buffer;
	char i;
	char tmp;

	// p_stack = cc_stack_new(10, sizeof(char));

	buffer = alloca(10);
	cc_array_init(&array, buffer, 10, sizeof(char));
	cc_stack_init(&stack, &array);

	for (i = 0; i < 10; i++)
		assert(cc_stack_push(&stack, &i));

	for (i = 0; i < 10; i++) {
		assert(cc_stack_pop(&stack, &tmp));
		printf(">> %d\n", tmp);
	}

	// cc_stack_free(p_stack);

	return 0;
}
