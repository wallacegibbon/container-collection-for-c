#include "cc_list_map.h"
#include <stdio.h>

int main() {
	struct cc_list_map *map;

	/// Caution: The `&tmp` is used as `void *` to accapt data.
	/// So the size of tmp should NOT be shorter than `size_t`,
	/// or data nearby would be overwritten.
	size_t tmp;

	map = cc_list_map_new(NULL);
	if (map == NULL)
		return 1;

	check_ret(cc_list_map_set(map, (void *)1, (void *)'a'));
	check_ret(cc_map_set(map, (void *)2, (void *)'b'));
	check_ret(cc_map_set(map, (void *)9, (void *)'c'));

	cc_list_map_print(map, "\n");

	check_ret(cc_list_map_get(map, (void *)1, (void **)&tmp));
	check_ret(tmp == 'a');
	check_ret(cc_map_get(map, (void *)9, (void **)&tmp));
	check_ret(tmp == 'c');
	check_ret(cc_map_get(map, (void *)2, (void **)&tmp));
	check_ret(tmp == 'b');

	check_ret(cc_map_del(map, (void *)2, (void **)&tmp));
	check_ret(tmp == 'b');

	cc_list_map_print(map, "\n");

	cc_list_map_delete(map);

	return 0;
}
