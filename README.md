# acm-icpc-library

To check that every snippet compiles and rebuild `main.pdf` automatically on each commit, enable the git hooks once:

```sh
git config core.hooksPath .githooks
```

My code library (a.k.a. reference document) used in ACM-ICPC.

Execute command `make` to generate the target pdf file.

Execute command `make check` to compile every C++ snippet in `code/`. Definitions a snippet expects from elsewhere (`point_t`, `maxn`, ...) live in `check/stubs/<path>.h`.
