#include "check/prelude.h"
#include "code/divide-and-conquer-on-tree.cpp"
// check: every vertex is a centroid exactly once, each centroid splits its component into parts of size <= half,
// and a centroid decomposition counting pairs with dist <= K matches O(n^2) BFS
mt19937 rng(13); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
int n, K;
ll pairs(vector<int> d) { // pairs i < j with d[i] + d[j] <= K
	sort(d.begin(), d.end()); ll r = 0;
	for (int i = 0, j = d.size() - 1; i < j;) if (d[i] + d[j] <= K) r += j - i++; else --j;
	return r;
}
int main() {
	for (int it = 0; it < 3000; ++it) {
		n = rnd(1, it % 10 ? 40 : 2000), K = rnd(0, 10); vector<vector<int>> edge(n);
		for (int i = 1; i < n; ++i) { int p = rnd(0, 2) ? i - 1 : rnd(0, i - 1); edge[i].push_back(p), edge[p].push_back(i); }
		Centroid cd(edge);
		vector<int> chosen(n); ll got = 0; bool central = true;
		function<void(int, int, int, vector<int> &)> dists = [&](int u, int p, int d, vector<int> &out) {
			out.push_back(d); for (int v : edge[u]) if (v != p && !cd.rem[v]) dists(v, u, d + 1, out);
		};
		cd.solve(rnd(0, n - 1), [&](int c) {
			chosen[c]++;
			vector<int> all{0}; vector<vector<int>> parts;
			for (int v : edge[c]) if (!cd.rem[v]) { parts.emplace_back(); dists(v, c, 1, parts.back()); all.insert(all.end(), parts.back().begin(), parts.back().end()); }
			for (auto &d : parts) central &= 2 * d.size() <= all.size(), got -= pairs(d);
			got += pairs(all);
		});
		if (!central) { printf("FAIL it=%d n=%d: not a centroid\n", it, n); return 1; }
		ll want = 0;
		if (n <= 300) for (int s = 0; s < n; ++s) { // BFS from every vertex
			vector<int> d(n, -1); queue<int> q; q.push(s), d[s] = 0;
			while (!q.empty()) { int u = q.front(); q.pop(); for (int v : edge[u]) if (d[v] < 0) d[v] = d[u] + 1, q.push(v); }
			for (int t = s + 1; t < n; ++t) want += d[t] <= K;
		} else want = got;
		bool ok = got == want && count(chosen.begin(), chosen.end(), 1) == n;
		if (!ok) { printf("FAIL it=%d n=%d K=%d: got %lld want %lld\n", it, n, K, got, want); return 1; }
	}
	puts("OK divide-and-conquer-on-tree");
}
