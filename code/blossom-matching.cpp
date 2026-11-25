// Edmonds blossom: max matching in a general graph, vertices 0..n-1, O(V^3).
// add(u, v); run() returns the matching size; mt[u]: partner of u or -1.
struct Blossom {
	int n, t = 0;
	vector<vector<int>> g;
	vector<int> mt, pre, f, st, vis; // f: blossom base dsu, st: 1 even, 2 odd
	queue<int> q;
	Blossom(int n) : n(n), g(n), mt(n, -1), pre(n), f(n), st(n), vis(n) {}
	void add(int u, int v) { g[u].push_back(v), g[v].push_back(u); }
	int fd(int x) { return f[x] == x ? x : f[x] = fd(f[x]); }
	int lca(int u, int v) {
		for (++t;; swap(u, v))
			if (u >= 0) {
				if (vis[u = fd(u)] == t) return u;
				vis[u] = t, u = mt[u] < 0 ? -1 : pre[mt[u]];
			}
	}
	void blo(int u, int v, int a) {
		while (fd(u) != a) {
			pre[u] = v, v = mt[u];
			if (st[v] == 2) st[v] = 1, q.push(v);
			if (fd(u) == u) f[u] = a;
			if (fd(v) == v) f[v] = a;
			u = pre[v];
		}
	}
	bool aug(int s) {
		iota(all(f), 0), fill(all(st), 0), q = queue<int>({s}), st[s] = 1;
		while (!q.empty()) {
			int u = q.front();
			q.pop();
			for (int v : g[u])
				if (!st[v]) {
					pre[v] = u, st[v] = 2;
					if (mt[v] < 0) {
						for (int x = v, y; x >= 0; x = y)
							y = mt[pre[x]], mt[x] = pre[x], mt[pre[x]] = x;
						return 1;
					}
					st[mt[v]] = 1, q.push(mt[v]);
				} else if (st[v] == 1 && fd(u) != fd(v)) {
					int a = lca(u, v);
					blo(u, v, a), blo(v, u, a);
				}
		}
		return 0;
	}
	int run() {
		int r = 0;
		for (int i = 0; i < n; ++i)
			if (mt[i] < 0) r += aug(i);
		return r;
	}
};
