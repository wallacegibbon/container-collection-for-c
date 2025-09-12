#include "cc_hash_map.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int
main(void)
{
	cc_HashMap *map;
	cc_HashMapIter iter;
	cc_MapItem *item_tmp;
	uintptr_t tmp;
	size_t index;

	assert(!cc_hash_map_new(&map, 10, NULL, NULL));
	assert(!cc_hash_map_print(map, "\n"));

	assert(!cc_hash_map_iter_init(&iter, map));
	while (!cc_iter_next(&iter, &item_tmp, &index))
		cc_debug_print("(%d){%zu -> %zu} ", index, item_tmp->key,
				item_tmp->value);

	assert(!cc_hash_map_delete(map));

	return 0;
}
