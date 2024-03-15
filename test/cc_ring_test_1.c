#include "cc_ring.h"
#include <assert.h>

int main() {
	struct cc_ring *ring;
	char i, tmp;

	ring = cc_ring_new(8, sizeof(char));
	assert(cc_ring_space(ring) == 8);

	for (i = 0; i < 8; i++) {
		assert(cc_ring_append(ring, &i));
		assert(cc_ring_space(ring) == (size_t)(8 - i - 1));
	}

	assert(cc_ring_space(ring) == 0);
	assert(!cc_ring_append(ring, &i));

	for (i = 0; i < 8; i++) {
		assert(cc_ring_shift(ring, &tmp));
		assert(cc_ring_space(ring) == (size_t)i + 1);
	}

	assert(cc_ring_space(ring) == 8);
	assert(!cc_ring_shift(ring, &tmp));

	cc_ring_delete(ring, NULL);

	return 0;
}
