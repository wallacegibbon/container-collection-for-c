#include "cc_hash_map.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
  struct cc_hash_map *map;
  struct cc_hash_map_iter iter;
  struct cc_map_item *item_tmp;
  uintptr_t tmp;
  size_t index;

  assert (!cc_hash_map_new (&map, 10, NULL, NULL));
  assert (!cc_hash_map_print (map, "\n"));

  assert (!cc_hash_map_set (map, (void *)11, (void *)101, NULL));
  assert (!cc_hash_map_set (map, (void *)12, (void *)102, NULL));
  assert (!cc_hash_map_set (map, (void *)19, (void *)109, NULL));

  assert (!cc_map_set (map, (void *)22, (void *)202, NULL));
  assert (!cc_map_set (map, (void *)32, (void *)302, NULL));
  assert (!cc_map_set (map, (void *)41, (void *)401, NULL));
  assert (!cc_map_set (map, (void *)52, (void *)502, NULL));
  assert (!cc_map_set (map, (void *)63, (void *)603, NULL));
  assert (!cc_map_set (map, (void *)66, (void *)606, NULL));

  assert (cc_map_set_new (map, (void *)66, (void *)607));
  assert (!cc_map_set (map, (void *)66, (void *)608, NULL));

  assert (!cc_hash_map_print (map, "\n"));

  assert (!cc_hash_map_get (map, (void *)11, (void **)&tmp));
  assert (tmp == 101);

  assert (!cc_map_get (map, (void *)19, (void **)&tmp));
  assert (tmp == 109);
  assert (!cc_map_get (map, (void *)12, (void **)&tmp));
  assert (tmp == 102);

  assert (!cc_map_del (map, (void *)19, (void **)&item_tmp));
  assert (item_tmp->value == (void *)109);
  free (item_tmp);

  assert (!cc_hash_map_print (map, "\n"));

  assert (!cc_hash_map_iter_init (&iter, map));
  while (!cc_iter_next (&iter, &item_tmp, &index))
    cc_debug_print ("(%d){%zu -> %zu} ", index, item_tmp->key,
                    item_tmp->value);

  assert (!cc_hash_map_delete (map));

  return 0;
}
