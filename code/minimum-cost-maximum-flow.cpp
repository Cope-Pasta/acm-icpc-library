// Min-cost max-flow, SPFA augmenting paths, O(flow * VE). Vertices 0..n-1.
// No negative cycles. add(u, v, cap, cost); cost_flow(s, t) = {flow, cost}.
struct MCMF {
	struct E {
		int v, r, w;
	};
	int n;
	vector<E> e;
	vector<vector<int>> g;
	MCMF(int n) : n(n), g(n) {}
	void add(int u, int v, int r, int w) {
		g[u].push_back(e.size()), e.push_back({v, r, w});
		g[v].push_back(e.size()), e.push_back({u, 0, -w});
	}
	pair<int, int> cost_flow(int s, int t) {
		int flow = 0, cost = 0;
		while (1) {
			vector<int> d(n, maxint), p(n), in(n);
			queue<int> q;
			q.push(s), d[s] = 0;
			while (!q.empty()) {
				int u = q.front();
				q.pop(), in[u] = 0;
				for (int id : g[u]) {
					auto [v, r, w] = e[id];
					if (r && d[u] + w < d[v]) {
						d[v] = d[u] + w, p[v] = id;
						if (!in[v]) in[v] = 1, q.push(v);
					}
				}
			}
			if (d[t] == maxint) return {flow, cost};
			int f = maxint;
			for (int v = t; v != s; v = e[p[v] ^ 1].v) f = min(f, e[p[v]].r);
			for (int v = t; v != s; v = e[p[v] ^ 1].v)
				e[p[v]].r -= f, e[p[v] ^ 1].r += f;
			flow += f, cost += f * d[t];
		}
	}
};
