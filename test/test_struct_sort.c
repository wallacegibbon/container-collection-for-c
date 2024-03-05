#include "../src/cc_array.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct blah {
	const char *name;
	int age;
};

/// define a global array to simplify the initializing of the testing generic_sequence.
struct blah people[] = {{"Harry", 10}, {"Albus", 109}, {"Severus", 50}};

void blah_display(struct blah *self) {
	printf("name %s, age: %d\n", self->name, self->age);
}

static int cmp_name(struct blah *left, struct blah *right) {
	return strcmp(left->name, right->name);
}

static int cmp_age(struct blah *left, struct blah *right) {
	return left->age - right->age;
}

int main() {
	struct cc_array array;
	struct cc_array_iter iter;
	struct blah tmp;
	int i;

	cc_array_init(&array, alloca(3 * sizeof(struct blah)), 3, sizeof(struct blah));

	/// Pushing values to generic sequence
	for (i = 0; i < 3; i++)
		assert(cc_array_set(&array, i, &people[i]));

	printf("\n=== before sort:\n");
	cc_array_iter_init(&iter, &array);
	while (cc_array_iter_next(&iter, &tmp))
		blah_display(&tmp);

	// assert(cc_array_sort(s, (cc_cmp_fn)cmp_age));
	assert(cc_array_sort(&array, (cc_cmp_fn)cmp_name));

	printf("\n=== after sort:\n");
	cc_array_iter_init(&iter, &array);
	while (cc_array_iter_next(&iter, &tmp))
		blah_display(&tmp);

	putchar('\n');
	return 0;
}
