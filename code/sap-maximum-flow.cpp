// ISAP max flow (gap + current arc), vertices 0..n-1. add(u, v, cap); sap(s, t) returns max flow. O(V^2 E).
struct SAP {
	struct E {
		int v, r;
	};
	int n, s, t;
	vector<E> e;
	vector<vector<int>> g;
	vector<int> d, vd, cur;
	SAP(int n) : n(n), g(n) {}
	void add(int u, int v, int r) {
		g[u].push_back(e.size()), e.push_back({v, r});
		g[v].push_back(e.size()), e.push_back({u, 0});
	}
	int aug(int u, int f) {
		if (u == t) return f;
		int res = 0;
		for (int &i = cur[u]; i < (int)g[u].size(); ++i) {
			int id = g[u][i], v = e[id].v;
			if (e[id].r && d[u] == d[v] + 1) {
				int x = aug(v, min(e[id].r, f - res));
				e[id].r -= x, e[id ^ 1].r += x, res += x;
				if (d[s] == n || res == f) return res;
			}
		}
		if (!--vd[d[u]]) d[s] = n;
		else cur[u] = 0, ++vd[++d[u]];
		return res;
	}
	int sap(int s_, int t_) {
		s = s_, t = t_, d.assign(n, 0), vd.assign(n + 1, 0), cur.assign(n, 0), vd[0] = n;
		int f = 0;
		while (d[s] < n) f += aug(s, maxint);
		return f;
	}
};
