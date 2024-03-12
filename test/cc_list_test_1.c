#include "cc_common.h"
#include "cc_list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	struct cc_list *list;
	struct cc_list_iter iter;
	size_t i;

	list = cc_list_new();
	if (list == NULL)
		return 1;

	for (i = 0; i < 10; i++) {
		assert(cc_list_append(list, (void *)i));
		printf("adding value %zu into list. (size: %zu)\n", i, list->root.size);
	}

	assert(!cc_list_insert(list, 11, (void *)55));
	assert(cc_list_insert(list, 10, (void *)66));
	assert(cc_list_insert(list, 0, (void *)77));
	assert(cc_list_insert(list, 3, (void *)88));

	printf(">>> the whole list values:\n\t");
	// cc_list_iter_init(&iter, list, 0);
	cc_list_iter_init(&iter, list, 1);
	while (cc_iter_next(&iter, &i))
		printf("%zu ", i);
	printf("\n");

	assert(!cc_list_remove(list, 13, (void **)&i));
	assert(cc_list_remove(list, 12, (void **)&i));
	assert(i == 66);
	assert(cc_list_remove(list, 3, (void **)&i));
	assert(i == 88);
	assert(cc_list_remove(list, 0, (void **)&i));
	assert(i == 77);

	printf(">>> the whole list values:\n\t");
	cc_list_iter_init(&iter, list, 1);
	while (cc_iter_next(&iter, &i))
		printf("%zu ", i);
	printf("\n");

	cc_list_delete(list, NULL);
	return 0;
}
