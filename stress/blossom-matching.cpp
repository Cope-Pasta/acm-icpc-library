#include "check/prelude.h"
#include "code/blossom-matching.cpp"
// brute: bitmask DP maximum matching on general graphs
mt19937 rng(6); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
int main() {
	for (int it = 0; it < 20000; ++it) {
		n = rnd(1, 12); edge.assign(n, {});
		vector<vector<bool>> adj(n, vector<bool>(n));
		int p = rnd(1, 5);
		for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) if (rnd(0, 9) < p) {
			edge[i].push_back(j), edge[j].push_back(i), adj[i][j] = adj[j][i] = true;
			if (rnd(0, 9) == 0) edge[i].push_back(j), edge[j].push_back(i); // multi-edge
		}
		vector<int> dp(1 << n); // dp[S]: max matching inside S, pairing the lowest vertex
		for (int S = 1; S < 1 << n; ++S) {
			int i = __builtin_ctz(S), T = S ^ 1 << i; dp[S] = dp[T];
			for (int j = 0; j < n; ++j) if ((T >> j & 1) && adj[i][j]) dp[S] = max(dp[S], dp[T ^ 1 << j] + 1);
		}
		int r = blossom(), cnt = 0; bool ok = true;
		for (int u = 0; u < n; ++u) if (match[u] != -1) ok &= match[match[u]] == u && adj[u][match[u]], ++cnt;
		ok &= r == dp[(1 << n) - 1] && cnt == 2 * r;
		if (!ok) { printf("FAIL it=%d n=%d: blossom %d brute %d\n", it, n, r, dp[(1 << n) - 1]); return 1; }
	}
	puts("OK blossom-matching");
}
