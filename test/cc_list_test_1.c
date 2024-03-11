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
		assert(cc_list_add(list, (void *)i));
		printf("adding value %zu into list. (size: %zu)\n", i, list->root.size);
	}

	assert(!cc_list_insert(list, (void *)'a', 11));
	assert(cc_list_insert(list, (void *)'b', 10));
	assert(cc_list_insert(list, (void *)'c', 0));
	assert(cc_list_insert(list, (void *)'d', 3));

	printf(">>> the whole list values:\n\t");
	cc_list_iter_init(&iter, list, 0);
	// cc_list_iter_init(&iter, list, 1);
	while (cc_iter_next(&iter, &i))
		printf("%zu ", i);
	printf("\n");

	assert(!cc_list_remove(list, 13, NULL));
	assert(cc_list_remove(list, 12, NULL));
	assert(cc_list_remove(list, 3, NULL));
	assert(cc_list_remove(list, 0, NULL));

	printf(">>> the whole list values:\n\t");
	cc_list_iter_init(&iter, list, 0);
	while (cc_iter_next(&iter, &i))
		printf("%zu ", i);
	printf("\n");

	cc_list_delete(list, NULL);
	return 0;
}
