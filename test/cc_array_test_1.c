#include "cc_array.h"
#include <assert.h>
#include <math.h>

int main(void) {
	unsigned char data[10 * sizeof(float)];
	struct cc_array array;
	assert(!cc_array_init(&array, data, 10, sizeof(float)));

	for (float i = 0.27; i < 10; i++)
		assert(!cc_array_set(&array, i, &i));

	for (float i = 0.27; i < 10; i++) {
		float tmp;
		assert(!cc_array_get(&array, i, &tmp));
		assert(fabs(tmp - i) < 0.000001);
	}

	assert(!cc_array_reverse(&array, 3, 7));
	assert(!cc_array_reverse(&array, 0, array.elem_nums));

	struct cc_array_iter iter;
	assert(!cc_array_iter_init(&iter, &array));

	float *iter_tmp;
	size_t index;
	while (!cc_iter_next(&iter, &iter_tmp, &index))
		cc_debug_print("(index: %02d) %.9f\n", index, *iter_tmp);

	cc_debug_print("\n");

	return 0;
}
