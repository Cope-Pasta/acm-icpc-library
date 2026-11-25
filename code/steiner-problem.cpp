// Steiner's Problem: g: n*n matrix (0x3f3f3f3f if no edge, g[i][i] = 0), ts: vertices to be united, 0-indexed.
int steiner(vector<vector<int>> g, const vector<int> &ts) { // O(3^m*n+2^m*n^2+n^3)
	int n = g.size(), m = ts.size();
	for (int k = 0; k < n; ++k)
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				g[i][j] = min(g[i][j], g[i][k] + g[k][j]); // floyd
	vector<vector<int>> dp(1 << m, vector<int>(n, 0));
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			dp[1 << i][j] = g[ts[i]][j];
		}
	}
	for (int i = 1; i < (1 << m); ++i) {
		if (((i - 1) & i) != 0) {
			for (int j = 0; j < n; ++j) {
				dp[i][j] = maxint;
				for (int k = (i - 1) & i; k > 0; k = (k - 1) & i) {
					dp[i][j] = min(dp[i][j], dp[k][j] + dp[i ^ k][j]);
				}
			}
			for (int j = 0; j < n; ++j) {
				for (int k = 0; k < n; ++k) {
					dp[i][j] = min(dp[i][j], dp[i][k] + g[k][j]);
				}
			}
		}
	}
	return dp[(1 << m) - 1][ts[0]];
}
