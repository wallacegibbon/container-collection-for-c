#! /bin/sh

for f in test/*; do
	echo running test file "$f"...
	#make MEMCHECK=1 build/$(basename $f .c) || exit 1
	make build/$(basename $f .c) || exit 1
	echo
	echo
done

echo All test done.
