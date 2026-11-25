// Centroid decomposition, O(n log n) + work in f. g: tree adjacency, 0-indexed.
// solve(u, f) calls f(c) for the centroid c of u's component, then recurses.
// c is already removed in f: walk from c over !rem vertices.
struct Centroid {
	vector<vector<int>> g;
	vector<int> sz;
	vector<bool> rem;
	Centroid(const vector<vector<int>> &g) : g(g), sz(g.size()), rem(g.size()) {}
	int calc(int u, int p) {
		sz[u] = 1;
		for (int v : g[u])
			if (v != p && !rem[v]) sz[u] += calc(v, u);
		return sz[u];
	}
	int find(int u, int p, int tot) {
		for (int v : g[u])
			if (v != p && !rem[v] && 2 * sz[v] > tot) return find(v, u, tot);
		return u;
	}
	template <class F> void solve(int u, F f) {
		int c = find(u, -1, calc(u, -1));
		rem[c] = true, f(c);
		for (int v : g[c])
			if (!rem[v]) solve(v, f);
	}
};
