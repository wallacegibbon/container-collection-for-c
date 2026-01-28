#include "cc_array.h"
#include <assert.h>
#include <stdlib.h>

struct blah {
	int index;
	void *payload;
};

int main(void) {
	struct cc_array *array;
	assert(!cc_array_new(&array, 10, sizeof(struct blah)));

	for (int i = 0; i < 10; i++) {
		struct blah tmp = { .index = i, .payload = malloc(10) };
		assert(!cc_array_set(array, i, &tmp));
	}

	struct cc_array_iter iter;
	assert(!cc_array_iter_init(&iter, array));
	struct blah *iter_tmp;
	while (!cc_iter_next(&iter, (void **)&iter_tmp, NULL))
		free(iter_tmp->payload);

	assert(!cc_array_delete(array));

	return 0;
}
