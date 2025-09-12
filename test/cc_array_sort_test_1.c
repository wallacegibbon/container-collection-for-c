#include "cc_array.h"
#include "cc_array_sort.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static const char *sample = "A quick brown fox jumps over the lazy dog.";

static int
cmp_char(char *left, char *right)
{
	/*
	return *left - *right;
	*/
	return -(*left - *right);
}

static void
char_array_display(cc_Array *chars, const char *prefix)
{
	cc_ArrayIter iter;
	char *tmp;

	cc_debug_print("%s", prefix);
	assert(!cc_array_iter_init(&iter, chars));
	while (!cc_iter_next(&iter, &tmp, NULL))
		cc_debug_print("%c", *tmp);
}

int
main(void)
{
	cc_Array array;
	unsigned char buffer[10];
	int i;

	assert(!cc_array_init(&array, buffer, 10, 1));

	for (i = 0; i < 10; i++)
		assert(!cc_array_set(&array, i, (void *)&sample[i]));

	char_array_display(&array, "\nbefore sort: ");

	/* sort the sequence */
	/*
	assert(!cc_array_sort_bubble(&array, (cc_CmpFn)cmp_char));
	*/
	assert(!cc_array_sort_quick(&array, (cc_CmpFn)cmp_char));

	/*
	assert(!strncmp((const char *)array.buffer, "  Abcikqru", 10));
	*/
	assert(!strncmp((const char *)array.data, "urqkicbA  ", 10));

	char_array_display(&array, "\nafter sort: ");

	putchar('\n');
	return 0;
}
