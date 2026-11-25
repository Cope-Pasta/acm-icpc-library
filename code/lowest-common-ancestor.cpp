const int logn = 20; // vertices 0..n-1, parent[root] = root
vector<int> parent, depth;
vector<vector<int>> lca;

void initLCA(int n) {
	lca.assign(logn, vector<int>(n));
	lca[0] = parent;
	for (int j = 1; j < logn; ++j) {
		for (int i = 0; i < n; ++i) {
			lca[j][i] = lca[j - 1][lca[j - 1][i]];
		}
	}
}

int LCA(int x, int y) {
	if (depth[x] < depth[y]) swap(x, y);
	for (int i = logn - 1; i >= 0; --i) {
		if (depth[x] - (1 << i) >= depth[y]) x = lca[i][x];
	}
	if (x == y) return x;
	for (int i = logn - 1; i >= 0; --i) {
		if (lca[i][x] != lca[i][y]) {
			x = lca[i][x], y = lca[i][y];
		}
	}
	return lca[0][x];
}
