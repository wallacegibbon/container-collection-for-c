#ifndef __CC_STRING_BUILDER_H
#define __CC_STRING_BUILDER_H

#include "cc_array_chain.h"

typedef struct cc_StrBuilder cc_StrBuilder;

struct cc_StrBuilder {
	cc_ArrayChain *chain;
};

int cc_str_builder_new(cc_StrBuilder **self);
int cc_str_builder_delete(cc_StrBuilder *self);
int cc_str_builder_to_string(cc_StrBuilder *self, char **result);
int cc_str_builder_append(cc_StrBuilder *self, char *s, size_t size);
int cc_str_builder_append_str(cc_StrBuilder *self, char *s);
int cc_string_concat(char **result, int n, ...);

#endif
