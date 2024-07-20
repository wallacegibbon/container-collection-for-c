#include "cc_string_builder.h"
#include "cc_array.h"
#include "cc_array_chain.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

int cc_string_builder_to_string(struct cc_string_builder *self, char **result)
{
	struct cc_array *arr;
	char zero = '\0';

	if (cc_array_chain_to_array(self->chain, &arr, 1))
		goto fail1;
	if (cc_array_set(arr, arr->elem_nums - 1, &zero))
		goto fail2;

	*result = (char *)arr->data;

	if (cc_array_delete_keep_data(arr))
		goto fail2;

	return 0;

fail2:
	cc_array_delete(arr);
fail1:
	return 1;
}

/// When `s` is NULL, ignore and return success directly
int cc_string_builder_append_str(struct cc_string_builder *self, char *s)
{
	if (s == NULL)
		return 0;

	while (*s) {
		if (cc_array_chain_add_elem(self->chain, s++))
			return 1;
	}

	return 0;
}

/// When `s` is NULL, ignore and return success directly
int cc_string_builder_append(struct cc_string_builder *self, char *s, size_t size)
{
	if (s == NULL)
		return 0;

	return cc_array_chain_append(self->chain, s, size);
}

int cc_string_builder_new(struct cc_string_builder **self)
{
	struct cc_string_builder *tmp;

	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		goto fail1;
	if (cc_array_chain_new(&tmp->chain, 128, sizeof(char)))
		goto fail2;

	*self = tmp;
	return 0;

fail3:
	cc_array_chain_delete(tmp->chain);
fail2:
	free(tmp);
fail1:
	return 1;
}

int cc_string_builder_delete(struct cc_string_builder *self)
{
	if (cc_array_chain_delete(self->chain))
		return 1;

	free(self);
	return 0;
}

int cc_string_concat(char **result, int n, ...)
{
	struct cc_string_builder *string_builder;
	char *tmp;
	va_list args;

	if (cc_string_builder_new(&string_builder))
		goto fail1;

	va_start(args, n);
	while (n--) {
		if (cc_string_builder_append_str(string_builder, va_arg(args, char *)))
			goto fail2;
	}
	va_end(args);

	if (cc_string_builder_to_string(string_builder, result))
		goto fail2;
	if (cc_string_builder_delete(string_builder))
		goto fail1;

	return 0;

fail2:
	cc_string_builder_delete(string_builder);
fail1:
	return 1;
}
