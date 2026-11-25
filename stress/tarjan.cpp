#include "check/prelude.h"
#include "code/tarjan.cpp"
// undirected multigraphs with self-loops: cut vertices vs brute vertex removal, bridges vs brute edge removal
mt19937 rng(9); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
int main() {
	for (int it = 0; it < 40000; ++it) {
		int n = rnd(1, 9); vector<pair<int, int>> E;
		for (int k = rnd(0, 15); k--;) E.push_back({rnd(0, n - 1), rnd(0, n - 1)});
		auto comps = [&](int ban, int banE) {
			vector<vector<int>> g(n);
			for (int i = 0; i < (int)E.size(); ++i) if (i != banE) { auto [u, v] = E[i]; if (u != ban && v != ban) g[u].push_back(v), g[v].push_back(u); }
			vector<bool> vis(n); int c = 0;
			for (int s = 0; s < n; ++s) if (s != ban && !vis[s]) {
				++c; vector<int> st{s}; vis[s] = true;
				while (!st.empty()) { int u = st.back(); st.pop_back(); for (int v : g[u]) if (!vis[v]) vis[v] = true, st.push_back(v); }
			}
			return c;
		};
		vector<vector<int>> g(n);
		for (auto [u, v] : E) { g[u].push_back(v); if (u != v) g[v].push_back(u); }
		Lowlink L(g);
		int base = comps(-1, -1);
		for (int u = 0; u < n; ++u) {
			bool alone = true; for (auto [a, b] : E) alone &= (a != u || b == u) && (b != u || a == u); // u has no non-loop edge
			if (L.cut[u] != (comps(u, -1) > base - alone)) { printf("FAIL it=%d n=%d u=%d (cut vertex)\n", it, n, u); return 1; }
		}
		set<pair<int, int>> want, got;
		for (int i = 0; i < (int)E.size(); ++i) if (comps(-1, i) > base) want.insert(minmax(E[i].first, E[i].second));
		for (auto [u, v] : L.br) if (!got.insert(minmax(u, v)).second) { printf("FAIL it=%d: duplicate bridge\n", it); return 1; }
		if (want != got) { printf("FAIL it=%d n=%d (bridges)\n", it, n); for (auto [u, v] : E) printf("%d-%d ", u, v); puts(""); return 1; }
	}
	puts("OK tarjan");
}
