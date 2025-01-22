#include "cc_list.h"
#include <assert.h>
#include <stdint.h>

int
main (void)
{
  struct cc_list *list;
  uintptr_t i;

  assert (!cc_list_new (&list));

  assert (cc_list_remove_head (list, (void **)&i) == CC_LIST_EMPTY);
  assert (cc_list_remove_tail (list, (void **)&i) == CC_LIST_EMPTY);
  assert (cc_list_get_tail (list, (void **)&i) == CC_LIST_EMPTY);

  for (i = 0; i < 4; i++)
    {
      assert (!cc_list_insert_tail (list, (void *)i));
      assert (list->root.size == i + 1);
    }

  // cc_list_print(list, 1);

  assert (!cc_list_get_head (list, (void **)&i));
  assert (i == 0);
  assert (!cc_list_remove_head (list, (void **)&i));
  assert (i == 0);
  assert (!cc_list_get_tail (list, (void **)&i));
  assert (i == 3);
  assert (!cc_list_remove_tail (list, (void **)&i));
  assert (i == 3);
  assert (!cc_list_remove_tail (list, (void **)&i));
  assert (i == 2);
  assert (!cc_list_remove_tail (list, (void **)&i));
  assert (i == 1);

  assert (cc_list_remove_tail (list, (void **)&i) == CC_LIST_EMPTY);

  assert (!cc_list_delete (list));
  return 0;
}
