#include "cc_string_builder.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	struct cc_string_builder *builder;
	char *tmp;

	assert(!cc_string_builder_new(&builder));

	assert(!cc_string_builder_to_string(builder, &tmp));
	assert(strcmp(tmp, "") == 0);
	free(tmp);

	assert(!cc_string_builder_append(builder, "abc", 3));
	assert(!cc_string_builder_append(builder, "def", 3));
	assert(!cc_string_builder_append(builder, NULL, 3));
	assert(!cc_string_builder_append_str(builder, "ghi"));
	assert(!cc_string_builder_append_str(builder, "jkl"));
	assert(!cc_string_builder_append_str(builder, NULL));

	assert(!cc_string_builder_to_string(builder, &tmp));

	// cc_debug_print(">> \"%s\"\n", tmp);
	assert(strcmp(tmp, "abcdefghijkl") == 0);
	free(tmp);

	assert(!cc_string_builder_delete(builder));

	assert(!cc_string_concat(&tmp, 3, "abc", "def", "ghi", "jkl"));
	assert(strcmp(tmp, "abcdefghi") == 0);
	free(tmp);

	return 0;
}
