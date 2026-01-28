#include "cc_ring.h"
#include <assert.h>

int main(void) {
	struct cc_ring *ring;
	assert(!cc_ring_new(&ring, 8, sizeof(char)));
	assert(cc_ring_space(ring) == 8);
	assert(cc_ring_elem_nums(ring) == 0);

	char tmp;
	assert(cc_ring_dequeue(ring, &tmp) == CC_QUEUE_EMPTY);
	assert(cc_queue_dequeue(ring, (void **)&tmp) == CC_QUEUE_EMPTY);

	for (char i = 0; i < 8; i++) {
		//assert(!cc_ring_enqueue(ring, &i));
		assert(!cc_queue_enqueue(ring, &i));
		assert(cc_ring_elem_nums(ring) == (size_t)i + 1);
		assert(cc_ring_space(ring) == (size_t)(8 - i - 1));
	}

	assert(cc_ring_space(ring) == 0);
	assert(cc_ring_enqueue(ring, &tmp) == CC_QUEUE_FULL);
	assert(cc_queue_enqueue(ring, &tmp) == CC_QUEUE_FULL);

	assert(!cc_ring_peek(ring, &tmp));
	assert(tmp == 0);
	assert(!cc_ring_peek(ring, &tmp));
	assert(tmp == 0);
	assert(!cc_queue_peek(ring, (void **)&tmp));
	assert(tmp == 0);

	for (char i = 0; i < 8; i++) {
		//assert(!cc_ring_dequeue(ring, &tmp));
		assert(!cc_queue_dequeue(ring, (void **)&tmp));
		assert(tmp == i);
		assert(cc_ring_elem_nums(ring) == (size_t)(8 - i - 1));
		assert(cc_ring_space(ring) == (size_t)i + 1);
	}

	assert(cc_ring_elem_nums(ring) == 0);
	assert(cc_ring_space(ring) == 8);
	assert(cc_ring_dequeue(ring, &tmp) == CC_QUEUE_EMPTY);
	assert(cc_queue_dequeue(ring, (void **)&tmp) == CC_QUEUE_EMPTY);

	assert(!cc_ring_delete(ring));

	return 0;
}
