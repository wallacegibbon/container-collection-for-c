#ifndef __CC_STRING_BUILDER_H
#define __CC_STRING_BUILDER_H

#include "cc_array_chain.h"

struct cc_string_builder {
	struct cc_array_chain *chain;
};

int cc_string_builder_new(struct cc_string_builder **self);
int cc_string_builder_delete(struct cc_string_builder *self);

int cc_string_builder_to_string(struct cc_string_builder *self, char **result);
int cc_string_builder_append(struct cc_string_builder *self, char *s, size_t size);
int cc_string_builder_append_str(struct cc_string_builder *self, char *s);

int cc_string_concat(char **result, int n, ...);

#endif
