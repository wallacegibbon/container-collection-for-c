#include "cc_common.h"
#include "cc_list.h"
#include <corecrt.h>
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
		printf("adding value %zu into list. (size: %zu)\n", i, list->size);
		if (!cc_list_add(list, (void *)i))
			return 2;
	}

	// cc_list_iter_init(&iter, list, 0);
	cc_list_iter_init(&iter, list, 1);

	while (cc_iter_next(&iter, &i))
		printf("list node: %zu\n", i);

	cc_list_delete(list, NULL);
	return 0;
}
