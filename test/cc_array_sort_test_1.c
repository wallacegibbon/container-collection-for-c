#include "cc_array.h"
#include "cc_array_sort.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static const char *sample = "A quick brown fox jumps over the lazy dog.";

static int cmp_char(char *left, char *right) {
	// return *left - *right;
	return -(*left - *right);
}

static void char_array_display(struct cc_array *chars, const char *prefix) {
	struct cc_array_iter iter;
	char *tmp;

	cc_debug_print("%s", prefix);
	assert(!cc_array_iter_init(&iter, chars));
	while (!cc_iter_next(&iter, &tmp, NULL))
		cc_debug_print("%c", *tmp);
}

int main() {
	struct cc_array array;
	unsigned char buffer[10];
	int i;

	assert(!cc_array_init(&array, buffer, 10, 1));

	for (i = 0; i < 10; i++)
		assert(!cc_array_set(&array, i, (void *)&sample[i]));

	char_array_display(&array, "\nbefore sort: ");

	/// sort the sequence
	// assert(!cc_array_sort_bubble(&array, (cc_cmp_fn)cmp_char));
	assert(!cc_array_sort_quick(&array, (cc_cmp_fn_t)cmp_char));

	// assert(!strncmp((const char *)array.buffer, "  Abcikqru", 10));
	assert(!strncmp((const char *)array.buffer, "urqkicbA  ", 10));

	char_array_display(&array, "\nafter sort: ");

	putchar('\n');
	return 0;
}
