// Chu-Liu/Edmonds: min-cost arborescence (all vertices reachable from root), O(nm). 0-indexed, -1 if none.
struct edge_t {
	int u, v, w;
};
int arborescence(int n, int root, vector<edge_t> e) {
	for (int res = 0, k;;) {
		vector<int> in(n, maxint), pre(n), id(n, -1), vis(n, -1);
		for (auto [u, v, w] : e)
			if (u != v && w < in[v]) in[v] = w, pre[v] = u;
		in[root] = 0, k = 0;
		for (int i = 0; i < n; i++) {
			if (in[i] == maxint) return -1;
			res += in[i];
			int u = i;
			for (; u != root && vis[u] < 0; u = pre[u]) vis[u] = i;
			if (u != root && vis[u] == i) {
				for (int v = pre[u]; v != u; v = pre[v]) id[v] = k;
				id[u] = k++;
			}
		}
		if (!k) return res;
		for (int &x : id)
			if (x < 0) x = k++;
		for (auto &[u, v, w] : e) w -= in[v], u = id[u], v = id[v];
		n = k, root = id[root];
	}
}
