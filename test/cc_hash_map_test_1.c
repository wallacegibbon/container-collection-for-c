#include "cc_hash_map.h"
#include <assert.h>
#include <stdio.h>

int main() {
	struct cc_hash_map *map;
	size_t tmp;

	map = cc_hash_map_new(10, NULL, NULL);
	if (map == NULL)
		return 1;

	// cc_hash_map_print(map, "\n");

	assert(cc_hash_map_set(map, (void *)11, (void *)101));
	assert(cc_hash_map_set(map, (void *)12, (void *)102));
	assert(cc_hash_map_set(map, (void *)19, (void *)109));

	assert(cc_map_set(map, (void *)22, (void *)202));
	assert(cc_map_set(map, (void *)32, (void *)302));
	assert(cc_map_set(map, (void *)41, (void *)401));
	assert(cc_map_set(map, (void *)52, (void *)502));
	assert(cc_map_set(map, (void *)63, (void *)603));
	assert(cc_map_set(map, (void *)66, (void *)606));

	cc_hash_map_print(map, "\n");

	assert(cc_hash_map_get(map, (void *)11, (void **)&tmp));
	assert(tmp == 101);

	assert(cc_map_get(map, (void *)19, (void **)&tmp));
	assert(tmp == 109);
	assert(cc_map_get(map, (void *)12, (void **)&tmp));
	assert(tmp == 102);

	assert(cc_map_del(map, (void *)19, (void **)&tmp));
	assert(tmp == 109);

	cc_hash_map_print(map, "\n");

	cc_hash_map_delete(map);

	return 0;
}
