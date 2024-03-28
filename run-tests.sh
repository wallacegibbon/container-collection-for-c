#! /bin/sh

MAKE_OPT=""

if test $# -eq 1; then
	if test $1 = "memcheck"; then
		MAKE_OPT="MEMCHECK=1"
	else
		echo "usage: run-test.sh [memcheck]" 2>&1
		exit 1
	fi
fi

for f in test/*; do
	echo running test file "$f"...
	make $MAKE_OPT build/$(basename $f .c) || exit 1
	echo
	echo
done

echo All test done.

