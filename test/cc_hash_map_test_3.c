#include "cc_hash_map.h"
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

int main(void)
{
	struct cc_hash_map *map;
	struct cc_hash_map_iter iter;
	struct cc_map_item *item_tmp;
	uintptr_t tmp;
	size_t index;

	assert(!cc_hash_map_new(&map, 10, NULL, NULL));
	assert(!cc_hash_map_print(map, "\n"));

	assert(!cc_hash_map_iter_init(&iter, map));
	while (!cc_iter_next(&iter, &item_tmp, &index))
		cc_debug_print("(%d){%zu -> %zu} ", index, item_tmp->key, item_tmp->value);

	assert(!cc_hash_map_delete(map));

	return 0;
}
