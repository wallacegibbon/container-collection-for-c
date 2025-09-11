#include "cc_array.h"
#include "cc_array_sort.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct blah {
	const char *name;
	int age;
};

/*
 * define a global array to simplify the initializing of the testing
 * generic_sequence.
 */
static const struct blah people[] = {
	{ "Harry", 10 }, { "Albus", 109 }, { "Severus", 50 }
};

static void blah_display(struct blah *self)
{
	cc_debug_print("<name %s, age: %d> ", self->name, self->age);
}

static void blah_array_display(cc_Array *blahs, const char *prefix)
{
	cc_ArrayIter iter;
	struct blah *tmp;

	cc_debug_print("%s", prefix);
	assert(!cc_array_iter_init(&iter, blahs));
	while (!cc_iter_next(&iter, &tmp, NULL))
		blah_display(tmp);
}

static int cmp_name(struct blah *left, struct blah *right)
{
	/*
	return strcmp(left->name, right->name);
	*/
	return -strcmp(left->name, right->name);
}

static int cmp_age(struct blah *left, struct blah *right)
{
	/*
	return left->age - right->age;
	*/
	return -(left->age - right->age);
}

int main(void)
{
	cc_Array array;
	unsigned char buffer[3 * sizeof(struct blah)];
	int i;
	struct blah tmp;

	assert(!cc_array_init(&array, buffer, 3, sizeof(struct blah)));

	/* Pushing values to generic sequence */
	for (i = 0; i < 3; i++)
		assert(!cc_array_set(&array, i, (void *)&people[i]));

	blah_array_display(&array, "\nbefore sort:\n");

	assert(!cc_array_sort_bubble(&array, (cc_CmpFn)cmp_name));

	blah_array_display(&array, "\nafter sort by name:\n");

	assert(!cc_array_get(&array, 0, &tmp));
	assert(tmp.age == 50);

	assert(!cc_array_sort_bubble(&array, (cc_CmpFn)cmp_age));

	blah_array_display(&array, "\nafter sort by age:\n");

	assert(!cc_array_get(&array, 0, &tmp));
	assert(tmp.age == 109);

	putchar('\n');
	return 0;
}
