#include "cc_list_map.h"
#include <assert.h>
#include <stdio.h>

int main() {
	struct cc_list_map *map;

	/// Caution: The `&tmp` is used as `void *` to accapt data.
	/// So the size of `tmp` should NOT be smaller than `size_t`, or data nearby would be overwritten.
	size_t tmp;

	map = cc_list_map_new(NULL);
	assert(map != NULL);

	assert(!cc_list_map_set(map, (void *)1, (void *)'a'));
	assert(!cc_map_set(map, (void *)2, (void *)'b'));
	assert(!cc_map_set(map, (void *)9, (void *)'c'));

	assert(!cc_list_map_print(map, "\n"));

	assert(!cc_list_map_get(map, (void *)1, (void **)&tmp));
	assert(tmp == 'a');
	assert(!cc_map_get(map, (void *)9, (void **)&tmp));
	assert(tmp == 'c');
	assert(!cc_map_get(map, (void *)2, (void **)&tmp));
	assert(tmp == 'b');

	assert(!cc_map_del(map, (void *)2, (void **)&tmp));
	assert(tmp == 'b');

	assert(!cc_list_map_print(map, "\n"));

	assert(!cc_list_map_delete(map));

	return 0;
}
