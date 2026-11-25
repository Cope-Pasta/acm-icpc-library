# Agent instructions

ACM-ICPC reference library: C++ snippets in `code/`, typeset into `main.pdf` by `main.tex`.

## Code style

Strongly prefer 0-indexing, `std::vector` over fixed-size global arrays, and wrapping algorithms and their state in structs over loose globals.

Keep lines within 84 columns (tabs count as 4): wider lines wrap in the two-column PDF. `.clang-format` enforces this; write header comments as short lines, one fact each, rather than relying on comment reflow.

## Checks

Run all of these before committing, and fix anything that fails:

```sh
make format-check   # clang-format 19.1.7 (pip install clang-format==19.1.7); fix with `make format`
make check          # every code/*.cpp compiles; missing outside definitions go in check/stubs/<path>.h
make stress         # all stress tests in parallel (JOBS=n to limit); `./stress/run.sh <name> ...` runs specific ones
make                # main.pdf builds (xelatex)
```

Enable the git hooks once with `git config core.hooksPath .githooks`. The pre-commit hook runs format-check, check and the PDF build, and stages `main.pdf`; the pre-push hook runs `make stress`. CI runs format-check, check, stress and the PDF build.

## Stress tests

When you add or change a snippet in `code/`, add or update the matching `stress/<name>.cpp`:

- Include `check/prelude.h`, the snippet from `code/`, and `stress/test.h` if you need `EXPECT`/`near`.
- Compare against a brute force on random inputs from a seeded `mt19937`. Cover edge cases (empty, single element, degenerate geometry).
- On a mismatch, print the failing input and return non-zero. On success, print `OK <name>`.
- Keep each test well under the 120 s timeout. Tests run with ASan/UBSan.

New snippets also need a `\lstinputlisting` entry in `main.tex`.

## Commits

Use Conventional Commits with short, lowercase, imperative subjects and no trailing period:

```
feat: add suffix automaton
fix: handle k=1 in linear recurrence
refactor: 0-index suffix array
test: stress string snippets
docs: convex hull trick needs strictly increasing k
chore: drop java template
ci: compile snippets before building pdf
```

Make one logical change per commit. Commit a snippet and its stress test together, or as `feat:` followed by `test:`.
