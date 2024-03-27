#! /bin/sh

for f in test/*; do
	echo running test file "$f"...
	#make DEBUG=1 MEMCHECK=1 build/$(basename $f .c) || exit 1
	make DEBUG=1 build/$(basename $f .c) || exit 1
	echo
	echo
done

echo All test done.
