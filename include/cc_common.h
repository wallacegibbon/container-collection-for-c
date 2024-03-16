#ifndef __CC_COMMON_H
#define __CC_COMMON_H

#define CC_WITH_DEFAULT(VAR, DEFAULT_VAL) ((VAR) == NULL ? (DEFAULT_VAL) : (VAR))

#include <stddef.h>

typedef void (*cc_cleanup_fn)(void *value);
typedef int (*cc_cmp_fn)(void *left, void *right);
typedef size_t (*cc_hash_fn)(void *obj);

int cc_default_cmp_fn(void *left, void *right);
size_t cc_default_hash_fn(void *obj);

void cc_exit_info(int code, const char *format, ...);

#endif
