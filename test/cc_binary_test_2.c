#include "cc_binary.h"
#include "cc_common.h"
#include <assert.h>
#include <stdlib.h>

// clang-format off
enum blah_node_type {NUMBER, OP};
// clang-format on

struct blah_node {
	enum blah_node_type type;
	union {
		char op_sign;
		int number;
	};
};

struct blah_node *blah_node_new_number(int number)
{
	struct blah_node *self;
	self = malloc(sizeof(*self));
	if (self == NULL)
		return NULL;

	self->type = NUMBER;
	self->number = number;
	return self;
}

struct blah_node *blah_node_new_op(char op_sign)
{
	struct blah_node *self;
	self = malloc(sizeof(*self));
	if (self == NULL)
		return NULL;

	self->type = OP;
	self->op_sign = op_sign;
	return self;
}

int blah_node_print(struct blah_node *node)
{
	if (node == NULL)
		return 0;
	if (node->type == NUMBER)
		return cc_debug_print("%d", node->number);
	else
		return cc_debug_print("<%c>", node->op_sign);
}

int blah_node_delete(struct blah_node *self)
{
	free(self);
	return 0;
}

struct parser {
	char *input;
	struct cc_binary *root;
};

int parser_step(struct parser *self, int *error)
{
	struct blah_node *new_node;
	char c;

	*error = 0;
	c = *self->input++;
	if (c == '\0')
		return 1;

	if (c >= '0' && c <= '9')
		new_node = blah_node_new_number(c - '0');
	else
		new_node = blah_node_new_op(c);

	if (new_node == NULL) {
		*error = 2;
		goto fail;
	}

	if (cc_binary_insert_left(self->root, new_node)) {
		*error = 3;
		goto fail;
	}

	return 0;
fail:
	return 1;
}

int parser_parse(struct parser *self, struct cc_binary **result)
{
	int error;
	if (try_reset_double_p(result))
		return 255;

	while (!parser_step(self, &error) && !error)
		;

	*result = self->root;
	return error;
}

struct parser *parser_new(char *input)
{
	struct parser *self;
	if (input == NULL)
		goto fail1;

	self = malloc(sizeof(*self));
	if (self == NULL)
		goto fail1;

	self->input = input;
	self->root = cc_binary_new(NULL, NULL);
	if (self->root == NULL)
		goto fail2;

	return self;

fail2:
	free(self);
fail1:
	return NULL;
}

/// Delete the parser along with the result cc_binary tree.
int parser_delete(struct parser *self)
{
	struct cc_binary_iter *iter;
	struct blah_node **tmp;

	iter = cc_binary_iter_new(self->root->left, CC_TRAVERSE_DEPTH_LEFT);
	if (iter == NULL)
		return 1;
	while (!cc_iter_next(iter, &tmp, NULL))
		assert(!blah_node_delete(*tmp));

	if (cc_binary_iter_delete(iter))
		return 2;
	if (cc_binary_delete(self->root))
		return 3;

	free(self);
	return 0;
}

static char *expr1 = "1+2";

int main()
{
	struct parser *parser;
	struct cc_binary *result;

	parser = parser_new(expr1);
	assert(parser != NULL);

	cc_debug_print("parsing expression: \"%s\"\n", expr1);
	assert(!parser_parse(parser, &result));

	assert(!cc_binary_rotate_right(&result->left));
	assert(((struct blah_node *)result->left->data)->op_sign == '+');

	cc_binary_print(result, 0, (cc_simple_fn_1_t)blah_node_print);

	assert(!parser_delete(parser));
	return 0;
}
