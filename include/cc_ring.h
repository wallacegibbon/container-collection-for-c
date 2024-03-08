#ifndef __CC_RING_H
#define __CC_RING_H

#include "cc_array.h"
#include <stddef.h>

struct cc_ring {
	struct cc_array *data;
	size_t read_index;
	size_t write_index;
};

void cc_ring_init(struct cc_ring *self, struct cc_array *data);

#ifndef NO_MALLOC

struct cc_ring *cc_ring_new(size_t elem_nums, size_t elem_size);
void cc_ring_delete(struct cc_ring *self);

#endif

int cc_ring_put(struct cc_ring *self, void *item);
int cc_ring_get(struct cc_ring *self, void *item);

#endif
