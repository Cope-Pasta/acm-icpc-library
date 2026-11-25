#include "check/prelude.h"
#include "code/arborescence.cpp"
// brute: every non-root vertex picks one incoming edge; keep choices where all reach the root
mt19937 rng(8); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
int main() {
	for (int it = 0; it < 5000; ++it) {
		int n = rnd(1, 6), root = rnd(0, n - 1); vector<edge_t> E(rnd(0, 12));
		for (auto &[u, v, w] : E) u = rnd(0, n - 1), v = rnd(0, n - 1), w = rnd(0, 20);
		vector<vector<int>> in(n);
		for (int i = 0; i < (int)E.size(); ++i) if (E[i].u != E[i].v && E[i].v != root) in[E[i].v].push_back(i);
		int best = INT_MAX; vector<int> pick(n, -1);
		function<void(int, int)> go = [&](int v, int w) {
			if (v == n) {
				for (int i = 0; i < n; ++i) { int u = i, k = 0; while (u != root && k++ <= n) u = E[pick[u]].u; if (u != root) return; }
				best = min(best, w); return;
			}
			if (v == root) return go(v + 1, w);
			for (int id : in[v]) pick[v] = id, go(v + 1, w + E[id].w);
		};
		go(0, 0);
		if (best == INT_MAX) best = -1;
		int r = arborescence(n, root, E);
		if (r != best) { printf("FAIL it=%d n=%d root=%d: arb %d brute %d\n", it, n, root, r, best); for (auto [u, v, w] : E) printf("%d %d %d\n", u, v, w); return 1; }
	}
	puts("OK arborescence");
}
