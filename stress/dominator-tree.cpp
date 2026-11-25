#include "check/prelude.h"
#include "code/dominator-tree.cpp"
// brute: d dominates v iff v is unreachable from root once d is removed; idom = closest strict dominator
mt19937 rng(11); int n; vector<vector<int>> edge; int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
vector<bool> reach(int root, int ban) {
	vector<bool> vis(n); if (root == ban) return vis;
	vector<int> st{root}; vis[root] = true;
	while (!st.empty()) { int u = st.back(); st.pop_back(); for (int v : edge[u]) if (v != ban && !vis[v]) vis[v] = true, st.push_back(v); }
	return vis;
}
int main() {
	for (int it = 0; it < 20000; ++it) {
		n = rnd(1, 9); edge.assign(n, {}); int root = rnd(0, n - 1);
		for (int k = rnd(0, 20); k--;) edge[rnd(0, n - 1)].push_back(rnd(0, n - 1));
		Dominator D(edge, root);
		vector<bool> all = reach(root, -1);
		vector<vector<bool>> dom(n, vector<bool>(n)); // dom[d][v]
		for (int d = 0; d < n; ++d) { auto r = reach(root, d); for (int v = 0; v < n; ++v) dom[d][v] = all[v] && (d == v || !r[v]); }
		vector<int> got = D.dom, want(n, -1); want[root] = root;
		for (int v = 0; v < n; ++v) if (all[v] && v != root) // idom: strict dominator dominated by all other strict dominators
			for (int d = 0; d < n; ++d) if (d != v && dom[d][v]) { bool ok = true; for (int e = 0; e < n; ++e) if (e != v && dom[e][v] && !dom[e][d]) ok = false; if (ok) want[v] = d; }
		if (got != want) { printf("FAIL it=%d n=%d root=%d\n", it, n, root); return 1; }
	}
	puts("OK dominator-tree");
}
