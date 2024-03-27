#include "cc_ring.h"

int main() {
	struct cc_ring *ring;
	char i, tmp;

	ring = cc_ring_new(8, sizeof(char));
	check_ret(cc_ring_space(ring) == 8);

	for (i = 0; i < 8; i++) {
		check_ret(cc_ring_append(ring, &i));
		check_ret(cc_ring_space(ring) == (size_t)(8 - i - 1));
	}

	check_ret(cc_ring_space(ring) == 0);
	check_ret(!cc_ring_append(ring, &i));

	for (i = 0; i < 8; i++) {
		check_ret(cc_ring_shift(ring, &tmp));
		check_ret(cc_ring_space(ring) == (size_t)i + 1);
	}

	check_ret(cc_ring_space(ring) == 8);
	check_ret(!cc_ring_shift(ring, &tmp));

	cc_ring_delete(ring);

	return 0;
}
