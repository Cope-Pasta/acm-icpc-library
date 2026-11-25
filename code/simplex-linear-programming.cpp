// Simplex (Bland's rule): max{cx | Ax <= b, x >= 0}, a: n x m, b: n, c: m.
// Returns the optimum, maxdbl if unbounded, -maxdbl if infeasible. Needs eps.
double simplex(const vector<vector<double>> &a, const vector<double> &b,
    const vector<double> &c) {
	int n = b.size(), m = c.size() + 1, r = n, s = m - 1;
	vector<vector<double>> d(n + 2, vector<double>(m + 1));
	vector<int> ix(n + m);
	iota(all(ix), 0);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m - 1; ++j) d[i][j] = -a[i][j];
		d[i][m - 1] = 1, d[i][m] = b[i];
		if (d[r][m] > d[i][m]) r = i;
	}
	for (int j = 0; j < m - 1; ++j) d[n][j] = c[j];
	d[n + 1][m - 1] = -1;
	for (double dd;;) {
		if (r < n) {
			swap(ix[s], ix[r + m]), d[r][s] = 1 / d[r][s];
			for (int j = 0; j <= m; ++j)
				if (j != s) d[r][j] *= -d[r][s];
			for (int i = 0; i <= n + 1; ++i)
				if (i != r) {
					for (int j = 0; j <= m; ++j)
						if (j != s) d[i][j] += d[r][j] * d[i][s];
					d[i][s] *= d[r][s];
				}
		}
		r = s = -1;
		for (int j = 0; j < m; ++j)
			if ((s < 0 || ix[s] > ix[j]) &&
			    (d[n + 1][j] > eps || (d[n + 1][j] > -eps && d[n][j] > eps)))
				s = j;
		if (s < 0) break;
		for (int i = 0; i < n; ++i)
			if (d[i][s] < -eps &&
			    (r < 0 || (dd = d[r][m] / d[r][s] - d[i][m] / d[i][s]) < -eps ||
			        (dd < eps && ix[r + m] > ix[i + m])))
				r = i;
		if (r < 0) return maxdbl;
	}
	if (d[n + 1][m] < -eps) return -maxdbl;
	double ans = 0;
	for (int i = m; i < n + m; ++i)
		if (ix[i] < m - 1) ans += d[i - m][m] * c[ix[i]];
	return ans;
}
