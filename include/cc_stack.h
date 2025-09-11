#ifndef __CC_STACK_H
#define __CC_STACK_H

typedef int (*cc_StackPushFn)(void *self, void *data);
typedef int (*cc_StackPopFn)(void *self, void **result);
typedef int (*cc_StackPeekFn)(void *self, void **result);

typedef struct cc_StackI cc_StackI;

struct cc_StackI {
	cc_StackPushFn push;
	cc_StackPopFn pop;
	cc_StackPeekFn peek;
};

enum {
	CC_STACK_EMPTY = 0xFC10,
	CC_STACK_FULL = 0xFC20,
};

static inline int cc_stack_push(void *self, void *data)
{
	return (*(cc_StackI **)self)->push(self, data);
}

static inline int cc_stack_pop(void *self, void **result)
{
	return (*(cc_StackI **)self)->pop(self, result);
}

static inline int cc_stack_peek(void *self, void **result)
{
	return (*(cc_StackI **)self)->peek(self, result);
}

#endif
