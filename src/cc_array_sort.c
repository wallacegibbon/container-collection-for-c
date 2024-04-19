#include "cc_array_sort.h"

///-----------------------------------------------------------------------------
/// Quick sort (https://en.wikipedia.org/wiki/Quicksort)
///-----------------------------------------------------------------------------
static size_t cc_array_divide(struct cc_array *self, cc_cmp_fn_t cmp, size_t start, size_t end)
{
	size_t left, right, middle;

	middle = end - 1;
	right = end - 2;
	left = start;

	while (left < right) {
		while (cc_array_cmp(self, cmp, left, middle) <= 0 && left < right)
			left++;
		while (cc_array_cmp(self, cmp, right, middle) > 0 && left < right)
			right--;

		if (left != right)
			cc_array_swap(self, left, right);
	}

	if (cc_array_cmp(self, cmp, left, middle) > 0)
		cc_array_swap(self, left, middle);

	return left + 1;
}

static void cc_array_sort_quick_recur(struct cc_array *self, cc_cmp_fn_t cmp, size_t start, size_t end)
{
	size_t middle;
	if (start >= end - 1)
		return;

	middle = cc_array_divide(self, cmp, start, end);

	cc_array_sort_quick_recur(self, cmp, start, middle);
	cc_array_sort_quick_recur(self, cmp, middle, end);
}

int cc_array_sort_quick(struct cc_array *self, cc_cmp_fn_t cmp)
{
	if (cmp == NULL)
		return 1;

	cc_array_sort_quick_recur(self, cmp, 0, self->elem_nums);
	return 0;
}

///-----------------------------------------------------------------------------
/// Bubble sort (https://en.wikipedia.org/wiki/Bubble_sort)
///-----------------------------------------------------------------------------
int cc_array_sort_bubble(struct cc_array *self, cc_cmp_fn_t cmp)
{
	size_t i, j, k;

	if (cmp == NULL)
		return 1;

	for (i = 0; i < self->elem_nums - 1; i++)
		for (j = 0, k = 1; j < self->elem_nums - 1 - i; j++, k++)
			if (cc_array_cmp(self, cmp, j, k) > 0)
				cc_array_swap(self, j, k);
	return 0;
}
