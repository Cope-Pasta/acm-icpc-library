struct MCMF { // Minimum cost, maximum flow(ZKW version), vertices 0..n-1
	struct E { int v, r, w; };
	int n, s, t; vector<E> e; vector<vector<int>> g; vector<int> d; vector<bool> vis;
	MCMF(int n) : n(n), g(n), d(n), vis(n) {}
	void add(int u, int v, int r, int w) { // (u, v, r, w), (v, u, 0, -w)
		g[u].push_back(e.size()), e.push_back({v, r, w});
		g[v].push_back(e.size()), e.push_back({u, 0, -w});
	}
	void spfa() {
		queue<int> q;
		d.assign(n, maxint), vis.assign(n, false);
		d[s] = 0, q.push(s), vis[s] = true;
		while (!q.empty()) {
			int u = q.front();
			q.pop(), vis[u] = false;
			for (int id : g[u]) {
				int v = e[id].v;
				if (e[id].r && d[u] + e[id].w < d[v]) {
					d[v] = d[u] + e[id].w;
					if (!vis[v]) q.push(v), vis[v] = true;
				}
			}
		}
		int dt = d[t];
		for (int i = 0; i < n; ++i) d[i] = dt - d[i];
	}
	int augment(int u, int flow) {
		if (u == t) return flow;
		vis[u] = true;
		for (int id : g[u]) {
			int v = e[id].v;
			if (e[id].r && !vis[v] && d[v] + e[id].w == d[u]) {
				int temp = augment(v, min(flow, e[id].r));
				if (temp) {
					e[id].r -= temp, e[id ^ 1].r += temp;
					return temp;
				}
			}
		}
		return 0;
	}
	bool adjust() {
		int delta = maxint;
		for (int u = 0; u < n; ++u) {
			if (!vis[u]) continue;
			for (int id : g[u]) {
				int v = e[id].v;
				if (e[id].r && !vis[v] && d[v] + e[id].w > d[u]) {
					delta = min(delta, d[v] + e[id].w - d[u]);
				}
			}
		}
		if (delta == maxint) return false;
		for (int i = 0; i < n; ++i) {
			if (vis[i]) d[i] += delta;
		}
		vis.assign(n, false);
		return true;
	}
	pair<int, int> cost_flow(int s_, int t_) {
		s = s_, t = t_;
		int temp, flow = 0, cost = 0;
		spfa();
		do {
			while ((temp = augment(s, maxint))) {
				flow += temp;
				vis.assign(n, false);
			}
		} while (adjust());
		for (int i = 1; i < (int)e.size(); i += 2) cost += e[i].r * e[i - 1].w;
		return make_pair(flow, cost);
	}
};
