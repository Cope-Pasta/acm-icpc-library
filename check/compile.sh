#!/bin/bash
# Compile every C++ snippet in code/. Each file is wrapped as
#   check/prelude.h (precompiled) + check/stubs/<path>.h + code/<path>.cpp + check/stubs/<path>.post.h
# where the optional stubs supply whatever the snippet assumes is defined elsewhere.
set -u
cd "$(dirname "$0")/.."
export CXX=${CXX:-g++} out=.check-build
rm -rf "$out" && mkdir -p "$out/log"
cp check/prelude.h "$out/prelude.h"
"$CXX" -std=c++17 -x c++-header "$out/prelude.h" -o "$out/prelude.h.gch" || exit 1

compile() { # prints the file name on failure
	f=$1; rel=${f#code/}; name=${rel//\//_}; stub=check/stubs/${rel%.cpp}
	{
		if [ -f "$stub.h" ]; then echo "#include \"$stub.h\""; fi
		echo "#include \"$f\""
		if [ -f "$stub.post.h" ]; then echo "#include \"$stub.post.h\""; fi
	} >"$out/$name"
	"$CXX" -std=c++17 -I. -fsyntax-only -include "$out/prelude.h" "$out/$name" >"$out/log/$name.log" 2>&1 || echo "$f"
}
export -f compile
jobs=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
failed=$(git ls-files 'code/*.cpp' | xargs -P "$jobs" -I{} bash -c 'compile {}' | sort)
if [ -n "$failed" ]; then
	for f in $failed; do rel=${f#code/}; echo "==== $f"; cat "$out/log/${rel//\//_}.log"; done
	echo "$(echo "$failed" | wc -l | tr -d ' ') file(s) failed to compile" >&2
	exit 1
fi
echo "all snippets compile"
