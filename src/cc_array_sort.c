#include "cc_array_sort.h"

/*
Quick sort (https://en.wikipedia.org/wiki/Quicksort)
*/
static size_t
cc_array_divide(cc_Array *self, cc_CmpFn cmp, size_t start, size_t end)
{
	size_t l, r, m;

	m = end - 1;
	r = end - 2;
	l = start;

	while (l < r) {
		while (cc_array_cmp(self, cmp, l, m) <= 0 && l < r)
			l++;
		while (cc_array_cmp(self, cmp, r, m) > 0 && l < r)
			r--;

		if (l != r)
			cc_array_swap(self, l, r);
	}

	if (cc_array_cmp(self, cmp, l, m) > 0)
		cc_array_swap(self, l, m);

	return l + 1;
}

static void
cc_array_sort_quick_recur(cc_Array *self, cc_CmpFn cmp, size_t start,
		size_t end)
{
	size_t m;

	if (start >= end - 1)
		return;

	m = cc_array_divide(self, cmp, start, end);

	cc_array_sort_quick_recur(self, cmp, start, m);
	cc_array_sort_quick_recur(self, cmp, m, end);
}

int
cc_array_sort_quick(cc_Array *self, cc_CmpFn cmp)
{
	if (cmp == NULL)
		return 1;

	cc_array_sort_quick_recur(self, cmp, 0, self->elem_nums);
	return 0;
}

/*
Bubble sort (https://en.wikipedia.org/wiki/Bubble_sort)
*/
int
cc_array_sort_bubble(cc_Array *self, cc_CmpFn cmp)
{
	size_t i, j, k;

	if (cmp == NULL)
		return 1;

	for (i = 0; i < self->elem_nums - 1; i++) {
		for (j = 0, k = 1; j < self->elem_nums - 1 - i; j++, k++) {
			if (cc_array_cmp(self, cmp, j, k) > 0)
				cc_array_swap(self, j, k);
		}
	}
	return 0;
}
