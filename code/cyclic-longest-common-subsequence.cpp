// Cyclic LCS: max over rotations r of a of LCS(r, b), O(|a| |b|).
// LCS table of (a + a, b), rerooted row by row.
int clcs(const string &a, const string &b) {
	int m = a.size(), n = b.size(), ans = 0;
	// p: 1 left, 2 diagonal, 3 (or 0) up
	vector<vector<int>> d(2 * m + 1, vector<int>(n + 1)), p = d;
	for (int i = 0; i <= 2 * m; ++i)
		for (int j = 0; j <= n; ++j) {
			if (j) d[i][j] = d[i][j - 1], p[i][j] = 1;
			if (i && j && a[(i - 1) % m] == b[j - 1] &&
			    d[i - 1][j - 1] + 1 > d[i][j])
				d[i][j] = d[i - 1][j - 1] + 1, p[i][j] = 2;
			if (i && d[i - 1][j] > d[i][j]) d[i][j] = d[i - 1][j], p[i][j] = 3;
		}
	for (int r = 0; r < m; ++r) {
		if (r) { // reroot the path tree at (r, 0)
			int i = r, j = 1;
			while (j <= n && p[i][j] != 2) ++j;
			if (j <= n) {
				p[i][j] = 1;
				while (i < 2 * m && j < n)
					if (p[i + 1][j] == 3) p[++i][j] = 1;
					else if (p[i + 1][j + 1] == 2) p[++i][++j] = 1;
					else ++j;
				while (i < 2 * m && p[i + 1][j] == 3) p[++i][j] = 1;
			}
		}
		int i = r + m, j = n, l = 0;
		while (i != r || j) {
			int q = p[i][j];
			l += q == 2, i -= q != 1, j -= q == 1 || q == 2;
		}
		ans = max(ans, l);
	}
	return ans;
}
