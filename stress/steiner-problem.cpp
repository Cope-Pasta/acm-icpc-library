#include "check/prelude.h"
#include "code/steiner-problem.cpp"
// brute: min over vertex sets containing all terminals of the MST of the induced subgraph
mt19937 rng(12); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
int main() {
	for (int it = 0; it < 5000; ++it) {
		int n = rnd(1, 8), m = rnd(1, min(n, 4));
		vector<vector<int>> g(n, vector<int>(n, 0x3f3f3f3f));
		for (int i = 0; i < n; ++i) g[i][i] = 0;
		for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) if (rnd(0, 2)) g[i][j] = g[j][i] = rnd(1, 20);
		for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if (i != j && g[i][j] != 0x3f3f3f3f) g[i][j] = g[j][i] = min(g[i][j], g[j][i]);
		vector<int> p(n); iota(p.begin(), p.end(), 0); shuffle(p.begin(), p.end(), rng);
		vector<int> ts(p.begin(), p.begin() + m); int T = 0; for (int t : ts) T |= 1 << t;
		LL best = LLONG_MAX;
		for (int S = 1; S < 1 << n; ++S) if ((S & T) == T) { // Prim on induced subgraph
			vector<LL> d(n, LLONG_MAX); vector<bool> in(n); d[__builtin_ctz(S)] = 0; LL w = 0; bool ok = true;
			for (int k = 0; k < __builtin_popcount(S); ++k) {
				int u = -1; for (int v = 0; v < n; ++v) if ((S >> v & 1) && !in[v] && (u == -1 || d[v] < d[u])) u = v;
				if (d[u] == LLONG_MAX) { ok = false; break; }
				in[u] = true, w += d[u];
				for (int v = 0; v < n; ++v) if ((S >> v & 1) && g[u][v] != 0x3f3f3f3f) d[v] = min(d[v], (LL)g[u][v]);
			}
			if (ok) best = min(best, w);
		}
		if (best == LLONG_MAX) continue; // terminals disconnected: snippet result is meaningless
		int r = steiner(g, ts);
		if (r != best) { printf("FAIL it=%d n=%d m=%d: steiner %d brute %lld\n", it, n, m, r, best); return 1; }
	}
	puts("OK steiner-problem");
}
