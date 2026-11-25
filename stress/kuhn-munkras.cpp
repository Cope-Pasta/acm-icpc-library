#include "check/prelude.h"
#include "code/kuhn-munkras.cpp"
// brute: try every injection of the left side into the right side (n <= m)
mt19937 rng(5); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
int main() {
	for (int it = 0; it < 20000; ++it) {
		int m = rnd(1, 6), n = rnd(1, m); vector<vector<int>> g(n, vector<int>(m));
		for (auto &row : g) for (int &x : row) x = rnd(-20, 20);
		vector<int> p(m); iota(p.begin(), p.end(), 0);
		int best = INT_MIN;
		do { int s = 0; for (int i = 0; i < n; ++i) s += g[i][p[i]]; best = max(best, s); } while (next_permutation(p.begin(), p.end()));
		auto [r, q] = km(g); int s = 0; vector<bool> seen(m);
		bool ok = r == best && (int)q.size() == n;
		for (int i = 0; i < n && ok; ++i) ok &= q[i] >= 0 && q[i] < m && !seen[q[i]], seen[q[i]] = true, s += g[i][q[i]];
		ok &= s == best;
		if (!ok) { printf("FAIL it=%d n=%d m=%d: km %d matched %d brute %d\n", it, n, m, r, s, best); return 1; }
	}
	puts("OK kuhn-munkras");
}
