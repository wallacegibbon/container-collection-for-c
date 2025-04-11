## Introduction

Containers in `container-collection-for-c` are all weak containers if we store
pointers in them.

Which means they do NOT free those pointers when the container got deleted.


## Tools

Use [valgrind](https://github.com/pmem/valgrind) on Unix-like systems
or [drmemory](https://github.com/DynamoRIO/drmemory) on Windows
to check (potential) memory leaks.

For example:

Using valgrind:

```sh
make MEMCHECK=1 build/cc_list_test_1
```

Using drmemory:

```sh
make MEMCHECK=2 build/cc_list_test_1
```
