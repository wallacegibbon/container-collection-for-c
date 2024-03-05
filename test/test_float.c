#include "../src/cc_array.h"
#include <assert.h>
#include <stdio.h>

int main() {
	struct cc_array *s;
	struct cc_array_iter iter;

	float i;
	float tmp;

	s = cc_array_new(10, sizeof(float));
	if (s == NULL)
		return 1;

	for (i = 0; i < 10; i++)
		assert(cc_array_set(s, i, &i));

	for (i = 0; i < 10; i++) {
		assert(cc_array_get(s, i, &tmp));
		printf(">>1 %.2f\n", tmp);
	}

	cc_array_iter_init(&iter, s);
	while (cc_array_iter_next(&iter, &tmp))
		printf(">>2 %.2f\n", tmp);

	cc_array_free(s);
	return 0;
}
