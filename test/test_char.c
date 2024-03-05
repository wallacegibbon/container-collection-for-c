#include "../src/cc_stack.h"
#include <assert.h>
#include <stdio.h>

int main() {
	struct cc_stack *s;
	char i;
	char tmp;

	s = cc_stack_new(10, sizeof(char));
	if (s == NULL)
		return 1;

	for (i = 0; i < 10; i++)
		assert(cc_stack_push(s, &i));

	for (i = 0; i < 10; i++) {
		assert(cc_stack_pop(s, &tmp));
		printf(">> %d\n", tmp);
	}

	cc_stack_free(s);
	return 0;
}
