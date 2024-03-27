#include "cc_list.h"
#include <stdio.h>
#include <stdlib.h>

/// Example of using `cc_list` as a queue.

int main() {
	struct cc_list *list;
	size_t tmp;

	list = cc_list_new();
	if (list == NULL)
		return 1;

	check_ret(!cc_list_remove(list, 0, (void **)&tmp));

	check_ret(cc_list_append(list, (void *)1));
	check_ret(cc_list_append(list, (void *)2));
	check_ret(cc_list_append(list, (void *)3));

	check_ret(cc_list_remove(list, 0, (void **)&tmp));
	check_ret(tmp == 1);
	check_ret(cc_list_remove(list, 0, (void **)&tmp));
	check_ret(tmp == 2);
	check_ret(cc_list_remove(list, 0, (void **)&tmp));
	check_ret(tmp == 3);

	check_ret(!cc_list_remove(list, 0, (void **)&tmp));

	cc_list_delete(list);
	return 0;
}
