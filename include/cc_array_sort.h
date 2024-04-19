#ifndef __CC_ARRAY_SORT_H
#define __CC_ARRAY_SORT_H

#include "cc_array.h"
#include "cc_common.h"

int cc_array_sort_bubble(struct cc_array *self, cc_cmp_fn_t);
int cc_array_sort_quick(struct cc_array *self, cc_cmp_fn_t);

#endif
