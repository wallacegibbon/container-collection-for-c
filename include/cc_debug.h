#ifndef __CC_DEBUG_H
#define __CC_DEBUG_H

#include <stdio.h>
#include <stdlib.h>

#define CC_ENSURE(expr) (void)(!(expr) ||                                                             \
			       (fprintf(stderr, "=%s,%d= (%s) failed.\n", __FILE__, __LINE__, #expr), \
				exit(1),                                                              \
				0))

#endif
