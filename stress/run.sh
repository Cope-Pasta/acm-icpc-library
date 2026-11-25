#!/bin/bash
# Build and run stress tests: ./stress/run.sh [name ...] (default: all stress/*.cpp)
# Each test includes snippets from code/, compares them with a brute force on random
# inputs, prints "OK <name>" and exits 0, or prints the failing case and exits non-zero.
# Tests run in parallel, JOBS at a time (default: nproc); each test's output is printed when it finishes.
set -u
cd "$(dirname "$0")/.."
CXX=${CXX:-g++}
JOBS=${JOBS:-$(nproc 2>/dev/null || echo 4)}
out=.stress-build
export ASAN_OPTIONS=${ASAN_OPTIONS:-detect_leaks=0} # snippets never free their nodes
mkdir -p "$out"
if [ $# -eq 0 ]; then set -- $(cd stress && ls *.cpp | sed 's/\.cpp$//'); fi

run_one() {
	local t=$1
	if ! "$CXX" -std=c++17 -O2 -g -I. -fsanitize=address,undefined -fno-sanitize-recover=undefined -o "$out/$t" "stress/$t.cpp" 2>"$out/$t.log"; then
		echo "BUILD FAILED $t"; cat "$out/$t.log"; return 1
	fi
	if ! timeout 120 "$out/$t"; then echo "FAILED $t"; return 1; fi
}

finish() { # reap one finished job; print its output and count failures
	local pid
	wait -n -p pid || failed=$((failed + 1))
	cat "$out/${names[$pid]}.out"
	unset "names[$pid]"; running=$((running - 1))
}

declare -A names
failed=0 running=0
for t in "$@"; do
	t=${t%.cpp}; t=${t#stress/}
	while [ "$running" -ge "$JOBS" ]; do finish; done
	run_one "$t" >"$out/$t.out" 2>&1 &
	names[$!]=$t; running=$((running + 1))
done
while [ "$running" -gt 0 ]; do finish; done
[ "$failed" -eq 0 ] && echo "all stress tests passed" || { echo "$failed stress test(s) failed" >&2; exit 1; }
