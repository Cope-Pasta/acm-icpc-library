#!/bin/bash
# Build and run stress tests: ./stress/run.sh [name ...] (default: all stress/*.cpp)
# Each test includes snippets from code/, compares them with a brute force on random
# inputs, prints "OK <name>" and exits 0, or prints the failing case and exits non-zero.
set -u
cd "$(dirname "$0")/.."
CXX=${CXX:-g++}
out=.stress-build
export ASAN_OPTIONS=${ASAN_OPTIONS:-detect_leaks=0} # snippets never free their nodes
mkdir -p "$out"
if [ $# -eq 0 ]; then set -- $(cd stress && ls *.cpp | sed 's/\.cpp$//'); fi
failed=0
for t in "$@"; do
	t=${t%.cpp}; t=${t#stress/}
	if ! "$CXX" -std=c++17 -O2 -g -I. -fsanitize=address,undefined -fno-sanitize-recover=undefined -o "$out/$t" "stress/$t.cpp" 2>"$out/$t.log"; then
		echo "BUILD FAILED $t"; cat "$out/$t.log"; failed=$((failed + 1)); continue
	fi
	if ! timeout 120 "$out/$t"; then echo "FAILED $t"; failed=$((failed + 1)); fi
done
[ "$failed" -eq 0 ] && echo "all stress tests passed" || { echo "$failed stress test(s) failed" >&2; exit 1; }
