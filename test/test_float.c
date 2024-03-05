#include "cc_array.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	struct cc_array array;
	struct cc_array_iter iter;

	float i;
	float tmp;

	cc_array_init(&array, alloca(10 * sizeof(float)), 10, sizeof(float));

	for (i = 0; i < 10; i++)
		assert(cc_array_set(&array, i, &i));

	for (i = 0; i < 10; i++) {
		assert(cc_array_get(&array, i, &tmp));
		printf(">>1 %.2f\n", tmp);
	}

	cc_array_iter_init(&iter, &array);
	while (cc_array_iter_next(&iter, &tmp))
		printf(">>2 %.2f\n", tmp);

	return 0;
}
