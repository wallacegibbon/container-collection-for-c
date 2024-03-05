#include "../src/cc_array.h"
#include <assert.h>
#include <stdio.h>

const char *sample = "A quick brown fox jumps over the lazy dog.";

static int cmp_char(char *left, char *right) {
	return *left - *right;
}

int main() {
	struct cc_array *s;
	struct cc_array_iter iter;
	char tmp;
	int i;

	s = cc_array_new(10, sizeof(char));
	if (s == NULL)
		return 1;

	/// install the comparing function
	for (i = 0; i < 10; i++)
		assert(cc_array_set(s, i, (void *)&sample[i]));

	printf("\n=== before sort:\n");
	cc_array_iter_init(&iter, s);
	while (cc_array_iter_next(&iter, &tmp))
		putchar(tmp);

	/// sort the sequence
	assert(cc_array_sort(s, (cc_cmp_fn)cmp_char));

	printf("\n=== after sort:\n");
	cc_array_iter_init(&iter, s);
	while (cc_array_iter_next(&iter, &tmp))
		putchar(tmp);

	putchar('\n');

	cc_array_free(s);
	return 0;
}
