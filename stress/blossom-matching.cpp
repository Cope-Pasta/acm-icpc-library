#include "check/prelude.h"
#include "code/blossom-matching.cpp"
// brute: bitmask DP maximum matching on general graphs
mt19937 rng(6); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
int main() {
	for (int it = 0; it < 20000; ++it) {
		int n = rnd(1, 12); Blossom b(n);
		vector<vector<bool>> adj(n, vector<bool>(n));
		int p = rnd(1, 5);
		for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) if (rnd(0, 9) < p) {
			b.add(i, j), adj[i][j] = adj[j][i] = true;
			if (rnd(0, 9) == 0) b.add(i, j); // multi-edge
		}
		vector<int> dp(1 << n); // dp[S]: max matching inside S, pairing the lowest vertex
		for (int S = 1; S < 1 << n; ++S) {
			int i = __builtin_ctz(S), T = S ^ 1 << i; dp[S] = dp[T];
			for (int j = 0; j < n; ++j) if ((T >> j & 1) && adj[i][j]) dp[S] = max(dp[S], dp[T ^ 1 << j] + 1);
		}
		int r = b.run(), cnt = 0; bool ok = true;
		for (int u = 0; u < n; ++u) if (b.mt[u] != -1) ok &= b.mt[b.mt[u]] == u && adj[u][b.mt[u]], ++cnt;
		ok &= r == dp[(1 << n) - 1] && cnt == 2 * r;
		if (!ok) { printf("FAIL it=%d n=%d: blossom %d brute %d\n", it, n, r, dp[(1 << n) - 1]); return 1; }
	}
	puts("OK blossom-matching");
}
