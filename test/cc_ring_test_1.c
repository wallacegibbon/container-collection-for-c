#include "cc_ring.h"
#include <assert.h>
#include <stdio.h>

int main() {
	struct cc_ring *ring;
	char i, tmp;

	ring = cc_ring_new(8, sizeof(char));

	printf("ring space: %zu\n", cc_ring_space(ring));

	for (i = 'a'; i < 'i'; i++) {
		// printf(">> put returning: %d\n", cc_ring_put(ring, &i));
		assert(cc_ring_put(ring, &i));
		printf("ring space: %zu\n", cc_ring_space(ring));
	}

	fflush(stdout);

	assert(!cc_ring_put(ring, &i));
	assert(!cc_ring_put(ring, &i));

	for (i = 0; i < 8; i++) {
		assert(cc_ring_get(ring, &tmp));
		printf(">> %c\n", tmp);
		printf("ring space: %zu\n", cc_ring_space(ring));
	}

	fflush(stdout);

	assert(!cc_ring_get(ring, &tmp));

	cc_ring_delete(ring);

	return 0;
}
