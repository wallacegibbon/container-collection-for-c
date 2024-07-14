#include "cc_string_builder.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	struct cc_string_builder *builder;
	char *tmp;

	assert(!cc_string_builder_new(&builder));

	assert(!cc_string_builder_append(builder, "abc", 3));
	assert(!cc_string_builder_append(builder, "def", 3));
	assert(!cc_string_builder_append(builder, "ghi", 3));
	assert(!cc_string_builder_append(builder, "jkl", 3));

	assert(!cc_string_builder_to_string(builder, &tmp));

	// cc_debug_print(">> \"%s\"\n", tmp);
	assert(strcmp(tmp, "abcdefghijkl") == 0);

	assert(!cc_string_builder_delete(builder));

	free(tmp);

	return 0;
}
