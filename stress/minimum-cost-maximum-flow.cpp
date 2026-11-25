#include "check/prelude.h"
#include "code/minimum-cost-maximum-flow.cpp"
// brute: Bellman-Ford successive shortest paths (non-negative costs)
mt19937 rng(2); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
pair<int, int> ssp(int n, const vector<array<int, 4>> &es, int s, int t) {
	struct E { int v, r, w; }; vector<E> e; vector<vector<int>> g(n);
	for (auto [u, v, r, w] : es) g[u].push_back(e.size()), e.push_back({v, r, w}), g[v].push_back(e.size()), e.push_back({u, 0, -w});
	int flow = 0, cost = 0;
	while (true) {
		vector<LL> d(n, LLONG_MAX); vector<int> pe(n, -1); d[s] = 0;
		for (int k = 0; k < n; ++k) for (int u = 0; u < n; ++u) if (d[u] != LLONG_MAX)
			for (int id : g[u]) if (e[id].r && d[u] + e[id].w < d[e[id].v]) d[e[id].v] = d[u] + e[id].w, pe[e[id].v] = id;
		if (d[t] == LLONG_MAX) return {flow, cost};
		int f = INT_MAX;
		for (int v = t; v != s; v = e[pe[v] ^ 1].v) f = min(f, e[pe[v]].r);
		for (int v = t; v != s; v = e[pe[v] ^ 1].v) e[pe[v]].r -= f, e[pe[v] ^ 1].r += f;
		flow += f, cost += f * d[t];
	}
}
int main() {
	for (int it = 0; it < 20000; ++it) {
		int n = rnd(2, 8), m = rnd(0, 20), s = rnd(0, n - 1), t = rnd(0, n - 2); if (t >= s) ++t;
		vector<array<int, 4>> es(m);
		for (auto &[u, v, r, w] : es) u = rnd(0, n - 1), v = rnd(0, n - 1), r = rnd(0, 10), w = rnd(0, 10);
		MCMF g(n); for (auto [u, v, r, w] : es) g.add(u, v, r, w);
		auto a = g.cost_flow(s, t), b = ssp(n, es, s, t);
		if (a != b) { printf("FAIL it=%d n=%d s=%d t=%d: mcmf (%d,%d) brute (%d,%d)\n", it, n, s, t, a.first, a.second, b.first, b.second); return 1; }
	}
	puts("OK minimum-cost-maximum-flow");
}
