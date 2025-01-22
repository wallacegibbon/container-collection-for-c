#ifndef __CC_STACK_H
#define __CC_STACK_H

typedef int (*cc_stack_push_fn_t) (void *self, void *data);
typedef int (*cc_stack_pop_fn_t) (void *self, void **result);
typedef int (*cc_stack_peek_fn_t) (void *self, void **result);

struct cc_stack_i
{
  cc_stack_push_fn_t push;
  cc_stack_pop_fn_t pop;
  cc_stack_peek_fn_t peek;
};

enum
{
  CC_STACK_EMPTY = 0xFC10,
  CC_STACK_FULL = 0xFC20,
};

static inline int
cc_stack_push (void *self, void *data)
{
  return (*(struct cc_stack_i **)self)->push (self, data);
}

static inline int
cc_stack_pop (void *self, void **result)
{
  return (*(struct cc_stack_i **)self)->pop (self, result);
}

static inline int
cc_stack_peek (void *self, void **result)
{
  return (*(struct cc_stack_i **)self)->peek (self, result);
}

#endif
