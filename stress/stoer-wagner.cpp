#include "check/prelude.h"
#include "code/stoer-wagner.cpp"
// brute: minimum over all non-trivial vertex bipartitions
mt19937 rng(7); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
int main() {
	for (int it = 0; it < 20000; ++it) {
		int n = rnd(2, 9); vector<vector<int>> g(n, vector<int>(n));
		for (int i = 0; i < n; ++i) for (int j = i; j < n; ++j) if (rnd(0, 2)) g[i][j] = g[j][i] = rnd(0, 10);
		int best = INT_MAX;
		for (int S = 1; S < (1 << n) - 1; ++S) {
			int c = 0;
			for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if ((S >> i & 1) && !(S >> j & 1)) c += g[i][j];
			best = min(best, c);
		}
		int r = stoer_wagner(g);
		if (r != best) { printf("FAIL it=%d n=%d: sw %d brute %d\n", it, n, r, best); return 1; }
	}
	puts("OK stoer-wagner");
}
