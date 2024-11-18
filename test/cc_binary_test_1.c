#include "cc_binary.h"
#include "cc_common.h"
#include <assert.h>
#include <stdint.h>

int show_tree_elements(struct cc_binary *tree) {
    struct cc_binary_iter *iter;
    size_t *tmp, index;

    assert(!cc_binary_iter_new(&iter, tree, CC_TRAVERSE_DEPTH_LEFT));

    while (!cc_iter_next(iter, &tmp, &index))
        cc_debug_print("(%d)%u, ", index, *tmp);

    cc_debug_print("\n");

    assert(!cc_binary_iter_delete(iter));
    return 0;
}

static void print_1(void *data) {
    cc_debug_print("%ld", (uintptr_t)data);
}

int main(void) {
    struct cc_binary *root;

    assert(!cc_binary_new(&root, NULL, NULL));

    assert(!cc_binary_insert_left(root, (void **)1));
    assert(!cc_binary_insert_left(root, (void **)2));
    assert(!cc_binary_insert_left(root, (void **)3));
    assert(!cc_binary_insert_left(root, (void **)4));
    assert(!cc_binary_insert_right(root, (void **)5));
    assert(!cc_binary_insert_right(root, (void **)6));
    assert(!cc_binary_insert_right(root, (void **)7));

    assert(root->left->data == (void *)4);
    assert(root->left->left->data == (void *)3);
    assert(root->left->left->left->data == (void *)2);
    assert(root->left->left->left->left->data == (void *)1);
    assert(root->right->data == (void *)7);
    assert(root->right->right->data == (void *)6);
    assert(root->right->right->right->data == (void *)5);

    assert(!cc_binary_print(root, 0, print_1));

    show_tree_elements(root);

    assert(!cc_binary_rotate_left(&root->right));
    assert(!cc_binary_rotate_left(&root->right));
    assert(cc_binary_rotate_left(&root->right));

    assert(!cc_binary_rotate_right(&root->left));
    assert(!cc_binary_rotate_right(&root->left));
    assert(!cc_binary_rotate_right(&root->left));
    assert(cc_binary_rotate_right(&root->left));

    assert(root->left->data == (void *)1);
    assert(root->left->right->data == (void *)2);
    assert(root->left->right->right->data == (void *)3);
    assert(root->left->right->right->right->data == (void *)4);
    assert(root->right->data == (void *)5);
    assert(root->right->left->data == (void *)6);
    assert(root->right->left->left->data == (void *)7);

    assert(!cc_binary_print(root, 0, print_1));

    show_tree_elements(root);

    assert(!cc_binary_delete(root));
    return 0;
}
