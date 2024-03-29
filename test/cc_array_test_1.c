#include "cc_array.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>

int main() {
	struct cc_array array;
	struct cc_array_iter iter;
	uint8_t buffer[10 * sizeof(float)];
	size_t count;

	float i;
	float tmp;
	float *iter_tmp;

	assert(!cc_array_init(&array, buffer, 10, sizeof(float)));

	for (i = 0.27; i < 10; i++)
		assert(!cc_array_set(&array, i, &i));

	for (i = 0.27; i < 10; i++) {
		assert(!cc_array_get(&array, i, &tmp));
		assert(fabs(tmp - i) < 0.000001);
	}

	assert(!cc_array_iter_init(&iter, &array));
	while (!cc_iter_next(&iter, &iter_tmp, &count))
		cc_debug_print("(index: %02d) %.9f\n", count, *iter_tmp);

	cc_debug_print("\n");

	return 0;
}
