## Introduction

Containers in `container-collection-for-c` are all weak containers if you store pointers in them.
Which means they do NOT free those pointers when the container got deleted.


## Tools

Use [drmemory](https://github.com/DynamoRIO/drmemory) to check (potential) memory leaks on Windows,
or [valgrind](https://github.com/pmem/valgrind) on Unix.

For example:

Using drmemory:

```sh
make MEMCHECK=1 build/cc_list_test_1
```

Using valgrind:

```sh
make MEMCHECK=2 build/cc_list_test_1
```

