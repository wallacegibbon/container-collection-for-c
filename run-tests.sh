#! /bin/sh
# This script works with `cc-with-test.mk`.

MAKE_OPT=""

parse_arguments() {
	case "$1" in
	"memcheck=1"|"memcheck")
		MAKE_OPT="MEMCHECK=1" ;;
	"memcheck=2")
		MAKE_OPT="MEMCHECK=2" ;;
	*)
		echo "Usage: run-test.sh [memcheck[=1|2]]" 2>&1
		exit 1
	esac
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
