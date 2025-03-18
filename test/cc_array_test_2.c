#include "cc_array.h"
#include <assert.h>
#include <stdlib.h>

struct blah {
	int index;
	void *payload;
};

int main(void)
{
	struct cc_array *array;
	struct cc_array_iter iter;
	struct blah tmp;
	struct blah *iter_tmp;
	int i;

	assert(!cc_array_new(&array, 10, sizeof(struct blah)));

	for (i = 0; i < 10; i++) {
		tmp.index = i;
		tmp.payload = malloc(10);
		assert(!cc_array_set(array, i, &tmp));
	}

	assert(!cc_array_iter_init(&iter, array));
	while (!cc_iter_next(&iter, (void **)&iter_tmp, NULL))
		free(iter_tmp->payload);

	assert(!cc_array_delete(array));

	return 0;
}
