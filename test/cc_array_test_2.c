#include "cc_array.h"
#include "cc_common.h"
#include <stddef.h>
#include <stdlib.h>

struct blah {
	size_t index;
	void *payload;
};

void blah_free(struct blah *self) {
	free(self->payload);
}

int main() {
	struct cc_array *array;
	struct blah tmp;
	int i;

	array = cc_array_new(10, sizeof(tmp));
	for (i = 0; i < 10; i++) {
		tmp.index = i;
		tmp.payload = malloc(10);
		cc_array_set(array, i, &tmp);
	}

	// cc_array_delete(array, NULL);
	cc_array_delete(array, (cc_cleanup_fn)blah_free);

	return 0;
}
