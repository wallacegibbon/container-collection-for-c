#include "cc_list_map.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
	cc_ListMap *map;
	cc_MapItem *item_tmp;
	/*
	 * Caution: The `&tmp` is used as `void *` to accapt data.
	 * So the size of `tmp` should NOT be smaller than `uintptr_t`,
	 * or data nearby would be overwritten.
	 */
	uintptr_t tmp;

	assert(!cc_list_map_new(&map, NULL));
	/*
	assert(!cc_list_map_print(map, "\n"));
	*/

	assert(cc_map_get(map, (void *)2, (void **)&tmp) ==
			CC_MAP_KEY_NOT_FOUND);

	assert(!cc_list_map_set(map, (void *)1, (void *)'a', NULL));
	assert(!cc_map_set(map, (void *)2, (void *)'b', NULL));
	assert(!cc_map_set(map, (void *)9, (void *)'c', NULL));

	/*
	assert(!cc_list_map_print(map, "\n"));
	*/

	assert(!cc_list_map_get(map, (void *)1, (void **)&tmp));
	assert(tmp == 'a');
	assert(!cc_map_get(map, (void *)9, (void **)&tmp));
	assert(tmp == 'c');
	assert(!cc_map_get(map, (void *)2, (void **)&tmp));
	assert(tmp == 'b');

	assert(!cc_map_del(map, (void *)2, (void **)&item_tmp));
	assert(item_tmp->value == (void *)'b');
	free(item_tmp);

	/*
	assert(!cc_list_map_print(map, "\n"));
	*/

	assert(!cc_map_get(map, (void *)1, (void **)&tmp));
	assert(cc_map_get(map, (void *)2, (void **)&tmp) ==
			CC_MAP_KEY_NOT_FOUND);
	assert(!cc_map_get(map, (void *)9, (void **)&tmp));
	assert(cc_map_get(map, (void *)8, (void **)&tmp) ==
			CC_MAP_KEY_NOT_FOUND);

	assert(!cc_map_del(map, (void *)1, (void **)&item_tmp));
	assert(item_tmp->value == (void *)'a');
	free(item_tmp);

	/*
	assert(!cc_list_map_print(map, "\n"));
	*/

	assert(!cc_map_del(map, (void *)9, (void **)&item_tmp));
	assert(item_tmp->value == (void *)'c');
	free(item_tmp);

	assert(cc_map_del(map, (void *)9, (void **)&item_tmp) ==
			CC_MAP_KEY_NOT_FOUND);

	/*
	assert(!cc_list_map_print(map, "\n"));
	*/

	assert(!cc_list_map_delete(map));

	return 0;
}
