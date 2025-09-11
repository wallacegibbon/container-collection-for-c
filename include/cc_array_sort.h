#ifndef __CC_ARRAY_SORT_H
#define __CC_ARRAY_SORT_H

#include "cc_array.h"
#include "cc_common.h"

int cc_array_sort_bubble(cc_Array *self, cc_CmpFn);
int cc_array_sort_quick(cc_Array *self, cc_CmpFn);

#endif
