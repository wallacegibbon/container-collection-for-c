#include "cc_common.h"
#include "cc_hash_map.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

char s1[] = {"duplicated_key"};
char s2[] = {"duplicated_key"};
char s3[] = {"duplicated_key"};

size_t bkdr_hash(const char *s);
size_t dumb_hash(const char *s);

int main() {
	struct cc_hash_map *map;
	size_t tmp;

	/// When using `char *`s as keys, you should use `strcmp` as the `cmp` function.
	// map = cc_hash_map_new(20, NULL, (cc_hash_fn)bkdr_hash);

	map = cc_hash_map_new(20, (cc_cmp_fn)strcmp, (cc_hash_fn)bkdr_hash);
	// map = cc_hash_map_new(20, (cc_cmp_fn)strcmp, (cc_hash_fn)dumb_hash);
	// map = cc_hash_map_new(20, (cc_cmp_fn)strcmp, NULL);
	if (map == NULL)
		return 1;

	// cc_hash_map_print(map, "\n");

	assert(cc_map_set(map, "lazy_dog", (void *)1));
	assert(cc_map_set(map, "quick_fox", (void *)2));
	assert(cc_map_set(map, "dumb_bunny", (void *)3));
	assert(cc_map_set(map, "sneaky_fox", (void *)4));
	assert(cc_map_set(map, "slow_sloth", (void *)5));
	assert(cc_map_set(map, "strong_bull", (void *)6));
	assert(cc_map_set(map, "blah1", (void *)15));
	assert(cc_map_set(map, "blah2", (void *)16));
	assert(cc_map_set(map, "blah3", (void *)17));
	assert(cc_map_set(map, "blah4", (void *)18));
	assert(cc_map_set(map, "blah5", (void *)19));
	assert(cc_map_set(map, "blah6", (void *)20));
	assert(cc_map_set(map, "blah7", (void *)31));
	assert(cc_map_set(map, "blah8", (void *)32));
	assert(cc_map_set(map, "blah9", (void *)33));
	assert(cc_map_set(map, "blah10", (void *)34));
	assert(cc_map_set(map, "blah11", (void *)35));
	assert(cc_map_set(map, "blah12", (void *)36));
	assert(cc_map_set(map, s1, (void *)40));
	assert(cc_map_set(map, s2, (void *)41));
	assert(cc_map_set(map, s3, (void *)42));

	cc_hash_map_print(map, "\n");

	assert(cc_map_get(map, "lazy_dog", (void **)&tmp));
	assert(tmp == 1);
	assert(cc_map_get(map, "strong_bull", (void **)&tmp));
	assert(tmp == 6);

	cc_hash_map_delete(map);

	return 0;
}

size_t bkdr_hash(const char *s) {
	size_t hash = 0;
	while (*s)
		/// 31, 131, 1313, 13131, ...
		hash = hash * 131 + *s++;

	return hash;
}

size_t dumb_hash(const char *s) {
	size_t hash = 0;
	while (*s)
		hash += *s++;

	return hash;
}
