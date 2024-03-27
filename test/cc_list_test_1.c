#include "cc_list.h"
#include <stdio.h>
#include <stdlib.h>

void show_number_list(struct cc_list *list, int direction) {
	struct cc_list_iter iter;
	size_t *tmp;
	size_t count;

	cc_debug_print(">>> the whole list values:\n\t");

	cc_list_iter_init(&iter, list, direction);

	while (cc_iter_next(&iter, &tmp, &count))
		cc_debug_print("%zu (%d) ", *tmp, count);

	cc_debug_print("\n");
}

int main() {
	struct cc_list *list;
	size_t i;

	list = cc_list_new();
	if (list == NULL)
		return 1;

	for (i = 0; i < 10; i++) {
		check_ret(cc_list_append(list, (void *)i));
		check_ret(list->root.size == i + 1);
	}

	check_ret(!cc_list_insert(list, 11, (void *)55));
	check_ret(cc_list_insert(list, 10, (void *)66));
	check_ret(cc_list_insert(list, 0, (void *)77));
	check_ret(cc_list_insert(list, 3, (void *)88));

	show_number_list(list, 1);

	check_ret(!cc_list_remove(list, 13, (void **)&i));
	check_ret(cc_list_remove(list, 12, (void **)&i));
	check_ret(i == 66);
	check_ret(cc_list_remove(list, 3, (void **)&i));
	check_ret(i == 88);
	check_ret(cc_list_remove(list, 0, (void **)&i));
	check_ret(i == 77);

	show_number_list(list, 1);

	cc_list_delete(list);
	return 0;
}
