#include "cc_array.h"
#include "cc_common.h"
#include "cc_iter.h"
#include "cc_list.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
	cc_List *list;
	cc_Array *arr;
	cc_ArrayIter iter;
	int *tmp_p, **pp;
	size_t i;

	assert(!cc_list_new(&list));

	assert(cc_list_remove_head(list, (void **)&tmp_p) == CC_LIST_EMPTY);
	assert(cc_list_remove_tail(list, (void **)&tmp_p) == CC_LIST_EMPTY);
	assert(cc_list_get_tail(list, (void **)&tmp_p) == CC_LIST_EMPTY);

	for (i = 0; i < 4; i++) {
		tmp_p = malloc(sizeof(*tmp_p));
		assert(tmp_p != NULL);
		*tmp_p = i * 10 + 1;
		assert(!cc_list_insert_tail(list, tmp_p));
		assert(list->root.size == i + 1);
	}

	/*
	cc_list_print(list, 1);
	*/

	assert(!cc_list_to_cc_array(list, &arr));
	assert(!cc_array_iter_init(&iter, arr));
	/*
	while (!cc_iter_next(&iter, &pp, &i))
		cc_debug_print("<%d, %d> ", i, **pp);
	cc_debug_print("\n");
	*/

	assert(!cc_array_delete(arr));

	assert(!cc_list_get_head(list, (void **)&tmp_p));
	assert(*tmp_p == 1);
	assert(!cc_list_remove_head(list, (void **)&tmp_p));
	assert(*tmp_p == 1);
	free(tmp_p);
	assert(!cc_list_get_tail(list, (void **)&tmp_p));
	assert(*tmp_p == 31);
	assert(!cc_list_remove_tail(list, (void **)&tmp_p));
	assert(*tmp_p == 31);
	free(tmp_p);
	assert(!cc_list_remove_tail(list, (void **)&tmp_p));
	assert(*tmp_p == 21);
	free(tmp_p);
	assert(!cc_list_remove_tail(list, (void **)&tmp_p));
	assert(*tmp_p == 11);
	free(tmp_p);

	assert(cc_list_remove_tail(list, (void **)&tmp_p) == CC_LIST_EMPTY);

	assert(!cc_list_delete(list));
	return 0;
}
