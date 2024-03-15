#ifndef __CC_COMMON_H
#define __CC_COMMON_H

typedef int (*cc_cmp_fn)(void *left, void *right);

typedef void (*cc_cleanup_fn)(void *value);

void exit_info(int code, const char *format, ...);

#endif
