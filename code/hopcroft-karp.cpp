// Hopcroft-Karp bipartite matching, O(E sqrt V). Left 0..n-1, right 0..m-1.
// g[u]: right neighbours of u. run() returns the matching size.
// l[u] / r[v]: partner of left u / right v, or -1.
struct HK {
	int n, m;
	vector<vector<int>> g;
	vector<int> l, r, lv;
	HK(int n, int m) : n(n), m(m), g(n) {}
	bool dfs(int u) {
		int d = lv[u];
		lv[u] = -1;
		for (int w : g[u])
			if (r[w] < 0 || (lv[r[w]] == d + 1 && dfs(r[w])))
				return l[u] = w, r[w] = u, 1;
		return 0;
	}
	int run() {
		l.assign(n, -1), r.assign(m, -1);
		for (int res = 0, d;; res += d) {
			lv.assign(n, -1);
			queue<int> q;
			for (int i = 0; i < n; ++i)
				if (l[i] < 0) lv[i] = 0, q.push(i);
			while (!q.empty()) {
				int u = q.front();
				q.pop();
				for (int w : g[u])
					if (r[w] >= 0 && lv[r[w]] < 0)
						lv[r[w]] = lv[u] + 1, q.push(r[w]);
			}
			d = 0;
			for (int i = 0; i < n; ++i) d += l[i] < 0 && dfs(i);
			if (!d) return res;
		}
	}
};
