struct SAP { // vertices 0..n-1, edge i ^ 1 is the reverse of edge i
	struct E { int v, r; };
	int n, s, t; vector<E> e; vector<vector<int>> g; vector<int> d, vd, cur;
	SAP(int n) : n(n), g(n) {}
	void add(int u, int v, int r) { // (u, v, r), (v, u, 0)
		g[u].push_back(e.size()), e.push_back({v, r});
		g[v].push_back(e.size()), e.push_back({u, 0});
	}
	int augment(int u, int flow) {
		if (u == t) return flow;
		int res = 0;
		for (int &i = cur[u]; i < (int)g[u].size(); ++i) {
			int id = g[u][i], v = e[id].v;
			if (e[id].r && d[u] == d[v] + 1) {
				int temp = augment(v, min(e[id].r, flow - res));
				e[id].r -= temp, e[id ^ 1].r += temp, res += temp;
				if (d[s] == n || res == flow) return res;
			}
		}
		if (--vd[d[u]] == 0) d[s] = n;
		else cur[u] = 0, ++vd[++d[u]];
		return res;
	}
	int sap(int s_, int t_) {
		s = s_, t = t_;
		int flow = 0;
		d.assign(n, 0), vd.assign(n + 1, 0), cur.assign(n, 0);
		vd[0] = n;
		while (d[s] < n) flow += augment(s, maxint);
		return flow;
	}
};
