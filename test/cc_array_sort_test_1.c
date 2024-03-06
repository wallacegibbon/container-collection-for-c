#include "../src/cc_array.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

const char *sample = "A quick brown fox jumps over the lazy dog.";

static int cmp_char(char *left, char *right) {
	return *left - *right;
}

void char_array_display(struct cc_array *chars, const char *prefix) {
	struct cc_array_iter iter;
	char tmp;

	printf("%s", prefix);
	cc_array_iter_init(&iter, chars);

	while (cc_array_iter_next(&iter, &tmp))
		putchar(tmp);
}

int main() {
	struct cc_array array;
	int i;

	cc_array_init(&array, alloca(10), 10, 1);

	/// install the comparing function
	for (i = 0; i < 10; i++)
		assert(cc_array_set(&array, i, (void *)&sample[i]));

	char_array_display(&array, "\nbefore sort:\n");

	/// sort the sequence
	assert(cc_array_sort(&array, (cc_cmp_fn)cmp_char));

	char_array_display(&array, "\nafter sort:\n");

	putchar('\n');
	return 0;
}
