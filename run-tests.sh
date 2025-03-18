#! /bin/sh
# This script works with `cc-with-test.mk`. (But `cc-with-test.mk` is not used directly by this script)

MAKE_OPT=""

parse_arguments() {
	if test $1 = "memcheck=1"; then
		MAKE_OPT="MEMCHECK=1"
	elif test $1 = "memcheck=2"; then
		MAKE_OPT="MEMCHECK=2"
	else
		echo "usage: run-test.sh [memcheck=1|2]" 2>&1
		exit 1
	fi
}

run_test() {
	echo [TEST] "$1"...
	gmake $MAKE_OPT build/$(basename $1 .c) || exit 1
	echo
	echo
}

if test $# -eq 1; then
	parse_arguments $1
fi

for f in test/*.c; do
	run_test $f
done

echo All test done.

