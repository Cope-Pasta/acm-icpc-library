struct edge_t {
	int u, v, w;
}; // minimum mean cycle(allow negative weight)
double mmc(int n, const vector<edge_t> &edge) { // vertices 0..n-1
	vector<vector<int>> dp(n + 1, vector<int>(n, maxint));
	dp[0].assign(n, 0);
	for (int i = 0; i < n; ++i) {
		for (auto [u, v, w] : edge) {
			if (dp[i][u] != maxint) dp[i + 1][v] = min(dp[i + 1][v], dp[i][u] + w);
		}
	}
	double res = maxdbl;
	for (int i = 0; i < n; ++i) {
		if (dp[n][i] == maxint) continue;
		double value = -maxdbl;
		for (int j = 0; j < n; ++j) {
			value = max(value, (double)(dp[n][i] - dp[j][i]) / (n - j));
		}
		res = min(res, value);
	}
	return res;
}
