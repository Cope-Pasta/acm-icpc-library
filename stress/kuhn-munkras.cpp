#include "check/prelude.h"
#include "code/kuhn-munkras.cpp"
// brute: try every injection of the left side into the right side (n <= m)
mt19937 rng(5); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
int main() {
	for (int it = 0; it < 20000; ++it) {
		m = rnd(1, 6), n = rnd(1, m); g.assign(n, vector<int>(m));
		for (auto &row : g) for (int &x : row) x = rnd(-20, 20);
		vector<int> p(m); iota(p.begin(), p.end(), 0);
		int best = INT_MIN;
		do { int s = 0; for (int i = 0; i < n; ++i) s += g[i][p[i]]; best = max(best, s); } while (next_permutation(p.begin(), p.end()));
		int r = km(), s = 0; vector<bool> seen(n);
		bool ok = r == best;
		for (int y = 0; y < m; ++y) if (match[y] != -1) ok &= !seen[match[y]], seen[match[y]] = true, s += g[match[y]][y];
		ok &= s == best && count(seen.begin(), seen.end(), true) == n;
		if (!ok) { printf("FAIL it=%d n=%d m=%d: km %d matched %d brute %d\n", it, n, m, r, s, best); return 1; }
	}
	puts("OK kuhn-munkras");
}
