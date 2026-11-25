// p(n): number of partitions of n into unordered positive parts (pentagonal number theorem), O(n sqrt n).
int partition(int n) {
	vector<int> dp(n + 1);
	dp[0] = 1;
	for (int i = 1; i <= n; ++i)
		for (int j = 1, r = 1, k; (k = j * (3 * j - 1) / 2) <= i; ++j, r = -r)
			dp[i] += r * (dp[i - k] + (k + j <= i ? dp[i - k - j] : 0));
	return dp[n];
}
