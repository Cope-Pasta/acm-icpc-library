void gauss(vector<vector<double>> &g) { // input: n * (n + 1) matrix, x[i] = g[i][n]
	int n = g.size();
	for (int i = 0; i < n; ++i) {
		double temp = 0;
		int pos = -1;
		for (int j = i; j < n; ++j) {
			if (fabs(g[j][i]) > temp) temp = fabs(g[j][i]), pos = j;
		}
		if (pos == -1) continue;
		swap(g[pos], g[i]);
		temp = g[i][i];
		for (int k = 0; k <= n; ++k) g[i][k] /= temp;
		for (int j = i + 1; j < n; ++j) {
			temp = g[j][i];
			for (int k = 0; k <= n; ++k) g[j][k] -= temp * g[i][k];
		}
	}
	for (int i = n - 1; i >= 0; --i) {
		for (int j = 0; j < i; ++j) {
			g[j][n] -= g[i][n] * g[j][i];
			g[j][i] = 0;
		}
	}
}