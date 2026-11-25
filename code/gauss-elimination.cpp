// Gauss-Jordan with partial pivoting, O(n^3). A nonsingular.
// g: n x (n + 1) augmented [A | b]; afterwards x[i] = g[i][n].
void gauss(vector<vector<double>> &g) {
	int n = g.size();
	for (int i = 0; i < n; ++i) {
		int p = i;
		for (int j = i; j < n; ++j)
			if (fabs(g[j][i]) > fabs(g[p][i])) p = j;
		swap(g[p], g[i]);
		if (g[i][i] == 0) continue;
		for (int k = n; k >= i; --k) g[i][k] /= g[i][i];
		for (int j = 0; j < n; ++j)
			if (j != i)
				for (int k = n; k >= i; --k) g[j][k] -= g[j][i] * g[i][k];
	}
}
