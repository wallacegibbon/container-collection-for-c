#ifndef __CC_COMMON_H
#define __CC_COMMON_H

typedef int (*cc_cmp_fn)(void *left, void *right);

typedef void (*cc_handle_fn)(void *value);

typedef int (*cc_iter_next_fn)(void *self, void *value);

struct cc_iter_i {
	cc_iter_next_fn next;
};

static inline int cc_iter_next(void *self, void *value) {
	return (*(struct cc_iter_i **)self)->next(self, value);
}

void exit_info(int code, const char *format, ...);

#endif
