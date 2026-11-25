#include "check/prelude.h"
#include "code/divide-and-conquer-on-tree.cpp"
// check: findCentre gives a centroid of the current component, work() leaves cut[] clean, and
// a centroid decomposition built from findCentre/bfs counts pairs with dist <= K like O(n^2) BFS
mt19937 rng(13); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
int n, K, depthMax; vector<int> chosen;
void dists(int u, int p, int d, vector<int> &out) { out.push_back(d); for (int v : edge[u]) if (v != p && !cut[v]) dists(v, u, d + 1, out); }
ll pairs(vector<int> d) { // pairs i < j with d[i] + d[j] <= K
	sort(d.begin(), d.end()); ll r = 0;
	for (int i = 0, j = d.size() - 1; i < j;) if (d[i] + d[j] <= K) r += j - i++; else --j;
	return r;
}
ll solve(int u, int lvl) {
	depthMax = max(depthMax, lvl);
	int c = findCentre(u), total = ver.size();
	chosen[c]++, cut[c] = true;
	for (int v : edge[c]) if (!cut[v]) { bfs(v); if (2 * (int)ver.size() > total) throw 1; }
	vector<int> all{0}; ll r = 0;
	for (int v : edge[c]) if (!cut[v]) { vector<int> d; dists(v, c, 1, d); r -= pairs(d); all.insert(all.end(), d.begin(), d.end()); }
	r += pairs(all);
	for (int v : edge[c]) if (!cut[v]) r += solve(v, lvl + 1);
	return r;
}
int main() {
	for (int it = 0; it < 3000; ++it) {
		n = rnd(1, it % 10 ? 40 : 2000), K = rnd(0, 10); edge.assign(n, {});
		sz.assign(n, 0), parent.assign(n, -1), cut.assign(n, false), chosen.assign(n, 0), depthMax = 0;
		for (int i = 1; i < n; ++i) { int p = rnd(0, 2) ? i - 1 : rnd(0, i - 1); edge[i].push_back(p), edge[p].push_back(i); }
		work(rnd(0, n - 1));
		if (count(cut.begin(), cut.end(), true)) { printf("FAIL it=%d: work() left cut[] set\n", it); return 1; }
		ll got;
		try { got = solve(rnd(0, n - 1), 1); } catch (int) { printf("FAIL it=%d n=%d: findCentre is not a centroid\n", it, n); return 1; }
		ll want = 0;
		if (n <= 300) for (int s = 0; s < n; ++s) { // BFS from every vertex
			vector<int> d(n, -1); queue<int> q; q.push(s), d[s] = 0;
			while (!q.empty()) { int u = q.front(); q.pop(); for (int v : edge[u]) if (d[v] < 0) d[v] = d[u] + 1, q.push(v); }
			for (int t = s + 1; t < n; ++t) want += d[t] <= K;
		} else want = got;
		bool ok = got == want && count(chosen.begin(), chosen.end(), 1) == n && (1 << (depthMax - 1)) <= n;
		if (!ok) { printf("FAIL it=%d n=%d K=%d: got %lld want %lld depth %d\n", it, n, K, got, want, depthMax); return 1; }
	}
	puts("OK divide-and-conquer-on-tree");
}
