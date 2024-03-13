#ifndef __CC_ITER_H
#define __CC_ITER_H

typedef int (*cc_iter_next_fn)(void *self, void *value);

struct cc_iter_i {
	cc_iter_next_fn next;
};

static inline int cc_iter_next(void *self, void *value) {
	return (*(struct cc_iter_i **)self)->next(self, value);
}

#endif
