#! /bin/sh
#
for f in test/*; do
	echo running test file "$f"...
	make build/$(basename $f .c) || exit 1
	echo
	echo
done
