#include "cc_common.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void exit_info(int code, const char *format, ...) {
	va_list args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);

	exit(code);
}

void cc_debug_print(const char *format, ...) {
	va_list args;

	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);

	fflush(stdout);
}

int cc_default_cmp_fn(void *left, void *right) {
	return (uintptr_t)left - (uintptr_t)right;
}

size_t cc_default_hash_fn(void *obj) {
	return (size_t)obj;
}
