#include "cc_common.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int cc_default_cmp_fn(void *left, void *right) {
	return (uintptr_t)left - (uintptr_t)right;
}

size_t cc_default_hash_fn(void *obj) {
	return (size_t)obj;
}

size_t cc_address_hash_fn(void *obj) {
	return (((unsigned long long)obj >> 3) + 1) * 131;
}

size_t cc_str_hash_fn_simple(void *obj) {
	size_t hash = 0;
	for (char *s = obj, c; (c = *s); s++)
		hash += c;
	return hash;
}

size_t cc_str_hash_fn_bkdr(void *obj) {
	size_t hash = 0;
	for (char *s = obj, c; (c = *s); s++)
		hash = hash * 131 + c; /// 31, 131, 1313, 13131, ...
	return hash;
}

int cc_default_delete_fn(void *obj) {
	free(obj);
	return 0;
}

void cc_print_n(char *s, int n) {
	while (n-- > 0)
		cc_debug_print("%s", s);
}

void cc_exit_info(int code, char *format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(code);
}

void cc_debug_print(char *format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
	fflush(stdout);
}
