// Hungarian (Kuhn-Munkres), O(n^2 m): max-weight assignment of the rows of
// n x m matrix g to distinct columns, n <= m. For min cost, negate g.
// Returns {weight, p}, p[i] = column of row i.
// (u, v, p, pre are 1-indexed; 0 is a sentinel.)
pair<int, vector<int>> km(const vector<vector<int>> &g) {
	int n = g.size(), m = g[0].size();
	vector<int> u(n + 1), v(m + 1), p(m + 1), pre(m + 1), res(n);
	for (int i = 1; i <= n; ++i) {
		int j0 = 0;
		p[0] = i;
		vector<int> d(m + 1, INT_MAX), done(m + 1);
		do {
			int i0 = p[j0], j1 = 0, delta = INT_MAX;
			done[j0] = 1;
			for (int j = 1; j <= m; ++j)
				if (!done[j]) {
					int c = -g[i0 - 1][j - 1] - u[i0] - v[j];
					if (c < d[j]) d[j] = c, pre[j] = j0;
					if (d[j] < delta) delta = d[j], j1 = j;
				}
			for (int j = 0; j <= m; ++j)
				if (done[j]) u[p[j]] += delta, v[j] -= delta;
				else d[j] -= delta;
			j0 = j1;
		} while (p[j0]);
		for (int j1; j0; j0 = j1) p[j0] = p[j1 = pre[j0]];
	}
	for (int j = 1; j <= m; ++j)
		if (p[j]) res[p[j] - 1] = j - 1;
	return {v[0], res};
}
