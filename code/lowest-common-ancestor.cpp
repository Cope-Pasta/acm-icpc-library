// Binary lifting LCA: O(n log n) build, O(log n) query. p[v]: parent (p[root] = root), d[v]: depth, 0-indexed.
struct LCA {
	vector<vector<int>> up;
	vector<int> d;
	LCA(const vector<int> &p, const vector<int> &d) : up(1, p), d(d) {
		for (int j = 1; (1 << j) < (int)p.size(); j++) {
			up.push_back(up[j - 1]);
			for (int &x : up[j]) x = up[j - 1][x];
		}
	}
	int query(int a, int b) {
		if (d[a] < d[b]) swap(a, b);
		for (int j = 0, k = d[a] - d[b]; k; j++, k >>= 1)
			if (k & 1) a = up[j][a];
		if (a == b) return a;
		for (int j = up.size() - 1; j >= 0; j--)
			if (up[j][a] != up[j][b]) a = up[j][a], b = up[j][b];
		return up[0][a];
	}
};
