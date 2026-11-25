#include "check/prelude.h"
#include "code/hopcroft-karp.cpp"
// brute: bitmask DP over left vertices / used right vertices
mt19937 rng(4); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
int main() {
	for (int it = 0; it < 20000; ++it) {
		n = rnd(0, 8), m = rnd(0, 8); edge.assign(n, {});
		vector<int> adj(n);
		for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) if (rnd(0, 2) == 0) edge[i].push_back(j), adj[i] |= 1 << j;
		vector<int> dp(1 << m, -1); dp[0] = 0; // dp over masks after processing i left vertices
		for (int i = 0; i < n; ++i) {
			vector<int> nd = dp;
			for (int S = 0; S < 1 << m; ++S) if (dp[S] >= 0) for (int j = 0; j < m; ++j)
				if ((adj[i] >> j & 1) && !(S >> j & 1)) nd[S | 1 << j] = max(nd[S | 1 << j], dp[S] + 1);
			dp = nd;
		}
		int best = *max_element(dp.begin(), dp.end()), r = hopcroftKarp();
		bool ok = r == best;
		for (int i = 0; i < n && ok; ++i) if (pr[i] != -1) ok = pr2[pr[i]] == i && (adj[i] >> pr[i] & 1);
		if (!ok) { printf("FAIL it=%d n=%d m=%d: hk %d brute %d\n", it, n, m, r, best); return 1; }
	}
	puts("OK hopcroft-karp");
}
