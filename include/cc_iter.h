#ifndef __CC_ITER_H
#define __CC_ITER_H

#include <stddef.h>

typedef int (*cc_IterNextFn)(void *self, void **item, size_t *index);

typedef struct cc_IterI cc_IterI;

struct cc_IterI {
	cc_IterNextFn next;
};

static inline int
cc_iter_next(void *self, void *item, size_t *index)
{
	return (*(cc_IterI **)self)->next(self, (void **)item, index);
}

#endif
