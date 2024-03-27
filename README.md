## Introduction

Containers in `container-collection-for-c` are all weak containers if you store pointers in them.
Which means they do NOT free those pointers when the container got deleted.


## Tools

Use [drmemory](https://github.com/DynamoRIO/drmemory) to check (potential) memory leaks.

> If you are on Linux, you can also use [valgrind](https://github.com/pmem/valgrind)

For example:

```sh
make DEBUG=1 MEMCHECK=1 build/cc_list_test_1
```

