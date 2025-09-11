#ifndef __CC_LIST_STACK_H
#define __CC_LIST_STACK_H

#include "cc_list.h"
#include "cc_stack.h"

typedef struct cc_ListStack cc_ListStack;

struct cc_ListStack {
	cc_StackI *interface;
	cc_List *list;
};

int cc_list_stack_new(cc_ListStack **self);
int cc_list_stack_delete(cc_ListStack *self);
int cc_list_stack_push(cc_ListStack *self, void *data);
int cc_list_stack_pop(cc_ListStack *self, void **result);
int cc_list_stack_peek(cc_ListStack *self, void **result);

#endif
