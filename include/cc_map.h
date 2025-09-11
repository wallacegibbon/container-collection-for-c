#ifndef __CC_MAP_H
#define __CC_MAP_H

typedef int (*cc_MapGetFn)(void *self, void *key, void **result);
typedef int (*cc_MapSetFn)(void *self, void *key, void *value, void **old_value);
typedef int (*cc_MapSetNewFn)(void *self, void *key, void *value);
typedef int (*cc_MapDelFn)(void *self, void *key, void **result);

typedef struct cc_MapI cc_MapI;

struct cc_MapI {
	cc_MapGetFn get;
	cc_MapSetFn set;
	cc_MapSetNewFn set_new;
	cc_MapDelFn del;
};

typedef struct cc_MapItem cc_MapItem;

struct cc_MapItem {
	void *key;
	void *value;
};

enum {
	CC_MAP_KEY_NOT_FOUND = 0xFB10,
	CC_MAP_KEY_ALREADY_EXIST = 0xFB20,
};

static inline int cc_map_get(void *self, void *key, void **result)
{
	return (*(cc_MapI **)self)->get(self, key, result);
}

static inline int cc_map_set(void *self, void *key, void *value,
		void **old_value)
{
	return (*(cc_MapI **)self)->set(self, key, value, old_value);
}

static inline int cc_map_set_new(void *self, void *key, void *value)
{
	return (*(cc_MapI **)self)->set_new(self, key, value);
}

static inline int cc_map_del(void *self, void *key, void **result)
{
	return (*(cc_MapI **)self)->del(self, key, result);
}

#endif
