#include "check/prelude.h"
#include "code/manhattan-mst.cpp"
// brute: Kruskal on all O(n^2) edges vs Kruskal on the snippet's candidate edges
int N;
int kruskal(vector<array<int, 3>> es) {
	sort(es.begin(), es.end());
	vector<int> f(N); iota(f.begin(), f.end(), 0);
	function<int(int)> g = [&](int x) { return f[x] == x ? x : f[x] = g(f[x]); };
	int s = 0;
	for (auto [w, u, v] : es) { assert(0 <= u && u < N && 0 <= v && v < N); if (g(u) != g(v)) f[g(u)] = g(v), s += w; }
	return s;
}
int dist(pair<int, int> a, pair<int, int> b) { return abs(a.first - b.first) + abs(a.second - b.second); }

int main() {
	unsigned seed = 1; mt19937 rng(seed);
	auto rnd = [&](int l, int r) { return int(rng() % (r - l + 1)) + l; };
	for (int it = 0; it < 5000; ++it) {
		int n = rnd(1, 40), R = rnd(0, 30);
		vector<pair<int, int>> p(n);
		for (auto &[x, y] : p) x = rnd(-R, R), y = rnd(-R, R);
		N = n;
		auto cand = manhattan(p);
		bool ok = (int)cand.size() <= 4 * n;
		for (auto [w, u, v] : cand) ok &= w == dist(p[u], p[v]);
		int got = kruskal(cand);
		vector<array<int, 3>> es;
		for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) es.push_back({dist(p[i], p[j]), i, j});
		int exp = kruskal(es);
		if (!ok || got != exp) {
			printf("seed %u it %d: got %d expected %d\n", seed, it, got, exp);
			for (auto [x, y] : p) printf("(%d,%d) ", x, y);
			puts(""); return 1;
		}
	}
	puts("OK manhattan-mst");
}
