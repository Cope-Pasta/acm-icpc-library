#include "check/prelude.h"
#include "code/minimum-mean-cycle.cpp"
// brute: enumerate all simple cycles (min mean cycle is attained by a simple cycle)
mt19937 rng(3); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
int n; vector<edge_t> E; double best; vector<bool> used;
void go(int s, int u, int len, int w) {
	for (auto [a, b, c] : E) if (a == u) {
		if (b == s) best = min(best, double(w + c) / (len + 1));
		else if (b > s && !used[b]) used[b] = true, go(s, b, len + 1, w + c), used[b] = false;
	}
}
int main() {
	for (int it = 0; it < 20000; ++it) {
		n = rnd(1, 6); E.resize(rnd(0, 12));
		for (auto &[u, v, w] : E) u = rnd(0, n - 1), v = rnd(0, n - 1), w = rnd(-10, 10);
		best = maxdbl, used.assign(n, false);
		for (int s = 0; s < n; ++s) go(s, s, 0, 0);
		double r = mmc(n, E);
		if (fabs(r - best) > 1e-9) { printf("FAIL it=%d n=%d: mmc %.6f brute %.6f\n", it, n, r, best); for (auto [u, v, w] : E) printf("%d %d %d\n", u, v, w); return 1; }
	}
	puts("OK minimum-mean-cycle");
}
