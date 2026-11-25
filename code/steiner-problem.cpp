// Steiner tree: min weight of a tree connecting terminals ts (connected), O(3^m n + 2^m n^2 + n^3).
// g: n*n matrix (maxint if no edge, g[i][i] = 0), 0-indexed. dp[S][v]: tree spanning S and v.
int steiner(vector<vector<int>> g, const vector<int> &ts) {
	int n = g.size(), m = ts.size();
	for (int k = 0; k < n; k++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++) g[i][j] = min(g[i][j], g[i][k] + g[k][j]);
	vector<vector<int>> dp(1 << m, vector<int>(n, maxint));
	for (int i = 0; i < m; i++) dp[1 << i] = g[ts[i]];
	for (int S = 1; S < 1 << m; S++) {
		for (int T = (S - 1) & S; T; T = (T - 1) & S)
			for (int j = 0; j < n; j++) dp[S][j] = min(dp[S][j], dp[T][j] + dp[S ^ T][j]);
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++) dp[S][j] = min(dp[S][j], dp[S][k] + g[k][j]);
	}
	return dp.back()[ts[0]];
}
