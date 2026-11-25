vector<vector<int>> pa;

int trace(int sx, int sy, int ex, int ey) {
	int l = 0;
	while (ex != sx || ey != sy) {
		if (pa[ex][ey] == 1) --ey;
		else if (pa[ex][ey] == 2) --ex, --ey, ++l;
		else --ex;
	}
	return l;
}

void reroot(int root, int m, int n) {
	int i = root, j = 1;
	while (j <= n && pa[i][j] != 2) ++j;
	if (j > n) return;
	pa[i][j] = 1;
	while (i < 2 * m && j < n) {
		if (pa[i + 1][j] == 3) pa[++i][j] = 1;
		else if (pa[i + 1][j + 1] == 2) pa[++i][++j] = 1;
		else ++j;
	}
	while (i < 2 * m && pa[i + 1][j] == 3) pa[++i][j] = 1;
}

void lcs(const string &a, const string &b) { // dp[i][j]: a[0...i-1], b[0...j-1]
	int m = a.size(), n = b.size();
	vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
	pa.assign(m + 1, vector<int>(n + 1, 0)), dp[0][0] = 0;
	for (int i = 0; i <= m; ++i) {
		for (int j = 0; j <= n; ++j) {
			if (j >= 1 && dp[i][j] < dp[i][j - 1]) dp[i][j] = dp[i][j - 1], pa[i][j] = 1;
			if (i >= 1 && j >= 1 && dp[i][j] < dp[i - 1][j - 1] + 1 && a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1, pa[i][j] = 2;
			if (i >= 1 && dp[i][j] < dp[i - 1][j]) dp[i][j] = dp[i - 1][j], pa[i][j] = 3;
		}
	}
}

int clcs(const string &a, const string &b) {
	int m = a.size(), n = b.size(), ans = 0;
	lcs(a + a, b);
	ans = trace(0, 0, m, n);
	for (int i = 1; i < m; ++i) {
		reroot(i, m, n);
		ans = max(ans, trace(i, 0, m + i, n));
	}
	return ans;
}
