#include "cc_hash_map.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char s1[] = { "duplicated_key" };
char s2[] = { "duplicated_key" };
char s3[] = { "duplicated_key" };

int
main (void)
{
  struct cc_hash_map *map;
  struct cc_map_item *tmp_item;
  uintptr_t tmp;

  /// When using `char *`s as keys, you should use `strcmp` as the `cmp`
  /// function.
  // assert(!cc_hash_map_new(&map, 20, NULL, cc_str_hash_fn_bkdr));
  assert (
      !cc_hash_map_new (&map, 20, (cc_cmp_fn_t)strcmp, cc_str_hash_fn_bkdr));
  // assert(!cc_hash_map_new(&map, 20, (cc_cmp_fn_t)strcmp,
  // cc_str_hash_fn_simple)); assert(!cc_hash_map_new(&map, 20,
  // (cc_cmp_fn_t)strcmp, NULL));

  assert (map != NULL);

  // assert(!cc_hash_map_print(map, "\n"));

  assert (!cc_map_set (map, "lazy_dog", (void *)1, NULL));
  assert (!cc_map_set (map, "quick_fox", (void *)2, NULL));
  assert (!cc_map_set (map, "dumb_bunny", (void *)3, NULL));
  assert (!cc_map_set (map, "sneaky_fox", (void *)4, NULL));
  assert (!cc_map_set (map, "slow_sloth", (void *)5, NULL));
  assert (!cc_map_set (map, "strong_bull", (void *)6, NULL));
  assert (!cc_map_set (map, "blah1", (void *)15, NULL));
  assert (!cc_map_set (map, "blah2", (void *)16, NULL));
  assert (!cc_map_set (map, "blah3", (void *)17, NULL));
  assert (!cc_map_set (map, "blah4", (void *)18, NULL));
  assert (!cc_map_set (map, "blah5", (void *)19, NULL));
  assert (!cc_map_set (map, "blah6", (void *)20, NULL));
  assert (!cc_map_set (map, "blah7", (void *)31, NULL));
  assert (!cc_map_set (map, "blah8", (void *)32, NULL));
  assert (!cc_map_set (map, "blah9", (void *)33, NULL));
  assert (!cc_map_set (map, "blah10", (void *)34, NULL));
  assert (!cc_map_set (map, "blah11", (void *)35, NULL));
  assert (!cc_map_set (map, "blah12", (void *)36, NULL));
  assert (!cc_map_set (map, s1, (void *)40, NULL));
  assert (!cc_map_set (map, s2, (void *)41, NULL));
  assert (!cc_map_set (map, s3, (void *)42, NULL));

  assert (!cc_hash_map_print (map, "\n"));

  assert (!cc_map_get (map, "lazy_dog", (void **)&tmp));
  assert (tmp == 1);
  assert (!cc_map_get (map, "strong_bull", (void **)&tmp));
  assert (tmp == 6);

  assert (!cc_map_del (map, s3, (void **)&tmp_item));
  free (tmp_item);
  assert (!cc_hash_map_print (map, "\n"));

  assert (!cc_hash_map_delete (map));

  return 0;
}
