#include "cc_array.h"
#include "cc_array_chain.h"
#include <assert.h>
#include <string.h>

int main(void) {
    struct cc_array_chain *chain;
    struct cc_array *r;
    char zero = '\0';

    assert(!cc_array_chain_new(&chain, 8, sizeof(char)));

    assert(!cc_array_chain_append(chain, "hello", 5));
    assert(!cc_array_chain_append(chain, ",", 1));
    assert(!cc_array_chain_append(chain, " ", 1));
    assert(!cc_array_chain_append(chain, "world. ", 7));
    assert(!cc_array_chain_append(
            chain, "A quick brown fox jumps over the lazy dog.", 42));

    assert(!cc_array_chain_to_array(chain, &r, 1));
    assert(!cc_array_set(r, r->elem_nums - 1, &zero));

    // cc_debug_print(">> %s\n", r->data);
    assert(!strcmp(
            (char *)r->data,
            "hello, world. A quick brown fox jumps over the lazy dog."));

    assert(!cc_array_chain_delete(chain));

    assert(!cc_array_delete(r));

    return 0;
}
