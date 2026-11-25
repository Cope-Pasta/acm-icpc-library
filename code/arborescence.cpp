struct edge_t { int u, v, w; }; // vertices 0..n-1, -1 if no arborescence

int arborescence(int n, int root, vector<edge_t> edge) {
	int res = 0;
	vector<int> ID(n), pre(n), in(n), vis(n);
	while (true) {
		ID.assign(n, -1), vis.assign(n, -1), in.assign(n, maxint);
		for (auto &[u, v, w] : edge) {
			if (u == v || in[v] <= w) continue;
			in[v] = w, pre[v] = u;
		}
		pre[root] = root, in[root] = 0;
		for (int i = 0; i < n; ++i) {
			if (in[i] == maxint) return -1;
			res += in[i];
		}
		int index = 0;
		for (int i = 0; i < n; ++i) {
			if (vis[i] != -1) continue;
			int u = i, v;
			while (vis[u] == -1) {
				vis[u] = i;
				u = pre[u];
			}
			if (vis[u] != i || u == root) continue;
			for (v = u, u = pre[u]; u != v; u = pre[u]) ID[u] = index;
			ID[v] = index++;
		}
		if (index == 0) return res;
		for (int i = 0; i < n; ++i) if (ID[i] == -1) ID[i] = index++;
		for (auto &[u, v, w] : edge) {
			w -= in[v];
			u = ID[u], v = ID[v];
		}
		n = index, root = ID[root];
	}
}
