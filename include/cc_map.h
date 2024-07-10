#ifndef __CC_MAP_H
#define __CC_MAP_H

typedef int (*cc_map_get_fn_t)(void *self, void *key, void **result);
typedef int (*cc_map_set_fn_t)(void *self, void *key, void *value, void **old_value);
typedef int (*cc_map_set_new_fn_t)(void *self, void *key, void *value);
typedef int (*cc_map_del_fn_t)(void *self, void *key, void **result);

struct cc_map_i {
	cc_map_get_fn_t get;
	cc_map_set_fn_t set;
	cc_map_set_new_fn_t set_new;
	cc_map_del_fn_t del;
};

struct cc_map_item {
	void *key;
	void *value;
};

static inline int cc_map_get(void *self, void *key, void **result)
{
	return (*(struct cc_map_i **)self)->get(self, key, result);
}

static inline int cc_map_set(void *self, void *key, void *value, void **old_value)
{
	return (*(struct cc_map_i **)self)->set(self, key, value, old_value);
}

static inline int cc_map_set_new(void *self, void *key, void *value)
{
	return (*(struct cc_map_i **)self)->set_new(self, key, value);
}

static inline int cc_map_del(void *self, void *key, void **result)
{
	return (*(struct cc_map_i **)self)->del(self, key, result);
}

#endif
