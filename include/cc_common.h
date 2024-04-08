#ifndef __CC_COMMON_H
#define __CC_COMMON_H

#define CC_WITH_DEFAULT(VAR, DEFAULT_VAL) ((VAR) == NULL ? (DEFAULT_VAL) : (VAR))

#include <stddef.h>

enum cc_traverse_direction {
	CC_TRAVERSE_DEPTH_LEFT = 4,
	CC_TRAVERSE_DEPTH_RIGHT,
	CC_TRAVERSE_BREADTH_LEFT = 8,
	CC_TRAVERSE_BREADTH_RIGHT,
};

static inline int try_reset_double_p(void *pointer) {
	if (pointer == NULL)
		return 1;

	*(void **)pointer = NULL;
	return 0;
}

typedef void (*cc_cleanup_fn)(void *value);
typedef int (*cc_cmp_fn)(void *left, void *right);
typedef size_t (*cc_hash_fn)(void *obj);

int cc_default_cmp_fn(void *left, void *right);

size_t cc_default_hash_fn(void *obj);
size_t cc_str_hash_fn_simple(void *obj);
size_t cc_str_hash_fn_bkdr(void *obj);

void cc_exit_info(int code, const char *format, ...);
void cc_debug_print(const char *format, ...);

void cc_print_n(const char *s, int n);

#endif
