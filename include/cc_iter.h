#ifndef __CC_ITER_H
#define __CC_ITER_H

#include <stddef.h>

typedef int (*cc_iter_next_fn_t)(void *self, void **item, size_t *index);

struct cc_iter_i {
	cc_iter_next_fn_t next;
};

static inline int cc_iter_next(void *self, void *item, size_t *index)
{
	return (*(struct cc_iter_i **)self)->next(self, (void **)item, index);
}

#endif
