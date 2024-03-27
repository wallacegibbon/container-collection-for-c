#include "cc_hash_map.h"
#include <stdio.h>
#include <string.h>

char s1[] = {"duplicated_key"};
char s2[] = {"duplicated_key"};
char s3[] = {"duplicated_key"};

int main() {
	struct cc_hash_map *map;
	size_t tmp;

	/// When using `char *`s as keys, you should use `strcmp` as the `cmp` function.
	// map = cc_hash_map_new(20, NULL, cc_str_hash_fn_bkdr);
	map = cc_hash_map_new(20, (cc_cmp_fn)strcmp, cc_str_hash_fn_bkdr);
	//  map = cc_hash_map_new(20, (cc_cmp_fn)strcmp, cc_str_hash_fn_simple);
	//   map = cc_hash_map_new(20, (cc_cmp_fn)strcmp, NULL);
	if (map == NULL)
		return 1;

	// cc_hash_map_print(map, "\n");

	check_ret(cc_map_set(map, "lazy_dog", (void *)1));
	check_ret(cc_map_set(map, "quick_fox", (void *)2));
	check_ret(cc_map_set(map, "dumb_bunny", (void *)3));
	check_ret(cc_map_set(map, "sneaky_fox", (void *)4));
	check_ret(cc_map_set(map, "slow_sloth", (void *)5));
	check_ret(cc_map_set(map, "strong_bull", (void *)6));
	check_ret(cc_map_set(map, "blah1", (void *)15));
	check_ret(cc_map_set(map, "blah2", (void *)16));
	check_ret(cc_map_set(map, "blah3", (void *)17));
	check_ret(cc_map_set(map, "blah4", (void *)18));
	check_ret(cc_map_set(map, "blah5", (void *)19));
	check_ret(cc_map_set(map, "blah6", (void *)20));
	check_ret(cc_map_set(map, "blah7", (void *)31));
	check_ret(cc_map_set(map, "blah8", (void *)32));
	check_ret(cc_map_set(map, "blah9", (void *)33));
	check_ret(cc_map_set(map, "blah10", (void *)34));
	check_ret(cc_map_set(map, "blah11", (void *)35));
	check_ret(cc_map_set(map, "blah12", (void *)36));
	check_ret(cc_map_set(map, s1, (void *)40));
	check_ret(cc_map_set(map, s2, (void *)41));
	check_ret(cc_map_set(map, s3, (void *)42));

	cc_hash_map_print(map, "\n");

	check_ret(cc_map_get(map, "lazy_dog", (void **)&tmp));
	check_ret(tmp == 1);
	check_ret(cc_map_get(map, "strong_bull", (void **)&tmp));
	check_ret(tmp == 6);

	check_ret(cc_map_del(map, s3, (void **)&tmp));
	cc_hash_map_print(map, "\n");

	cc_hash_map_delete(map);

	return 0;
}
