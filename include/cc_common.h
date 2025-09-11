#ifndef __CC_COMMON_H
#define __CC_COMMON_H

#define CC_WITH_DEFAULT(VAR, DEFAULT_VAL) \
		((VAR) == NULL ? (DEFAULT_VAL) : (VAR))

#include <stddef.h>

typedef enum cc_TraverseDirection cc_TraverseDirection;

enum cc_TraverseDirection {
	CC_TRAVERSE_DEPTH_LEFT = 4,
	CC_TRAVERSE_DEPTH_RIGHT,
	CC_TRAVERSE_BREADTH_LEFT = 8,
	CC_TRAVERSE_BREADTH_RIGHT,
};

static inline int try_reset_double_p(void *pointer)
{
	if (pointer == NULL)
		return 1;

	*(void **)pointer = NULL;
	return 0;
}

typedef int (*cc_DelFn)(void *obj);

typedef void (*cc_simple_fn_0_t)(void);
typedef void (*cc_simple_fn_1_t)(void *value);
typedef int (*cc_simple_fn_2_t)(void *value);

typedef int (*cc_CmpFn)(void *left, void *right);
typedef size_t (*cc_HashFn)(void *obj);

/* Simply do: left - right */
int cc_default_cmp_fn(void *left, void *right);

/* Return obj directly */
size_t cc_default_hash_fn(void *obj);

/* Address is 4/8 byte aligned. Do some calculations to make it a good hash. */
size_t cc_address_hash_fn(void *obj);

/* Calculate hash from string. */
size_t cc_str_hash_fn_simple(void *obj);

/* Calculate hash from string in the BKDR (31, 131, 1313, ...) way. */
size_t cc_str_hash_fn_bkdr(void *obj);

/* Call `free` and return 0. */
int cc_default_delete_fn(void *obj);

void cc_exit_info(int code, char *format, ...);
void cc_debug_print(char *format, ...);
void cc_print_n(char *s, int n);

#endif
