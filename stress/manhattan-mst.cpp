#include "check/prelude.h"
struct edge_t { int u, v, w; };
vector<edge_t> es; int N;
void add(int u, int v, int w) { assert(0 <= u && u < N && 0 <= v && v < N); es.push_back({u, v, w}); }
int kruskal() {
	sort(es.begin(), es.end(), [](edge_t a, edge_t b) { return a.w < b.w; });
	vector<int> f(N); iota(f.begin(), f.end(), 0);
	function<int(int)> g = [&](int x) { return f[x] == x ? x : f[x] = g(f[x]); };
	int s = 0;
	for (auto e : es) if (g(e.u) != g(e.v)) f[g(e.u)] = g(e.v), s += e.w;
	return s;
}
struct point;
int dist(const point &a, const point &b);
#include "code/manhattan-mst.cpp"
int dist(const point &a, const point &b) { return abs(a.x - b.x) + abs(a.y - b.y); }

int main() {
	unsigned seed = 1; mt19937 rng(seed);
	auto rnd = [&](int l, int r) { return int(rng() % (r - l + 1)) + l; };
	for (int it = 0; it < 5000; ++it) {
		int n = rnd(1, 40), R = rnd(0, 30);
		vector<point> p(n);
		for (auto &q : p) q.x = rnd(-R, R), q.y = rnd(-R, R);
		es.clear(), N = n;
		int got = manhattan(p);
		es.clear();
		for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) add(i, j, dist(p[i], p[j]));
		int exp = kruskal();
		if (got != exp) {
			printf("seed %u it %d: got %d expected %d\n", seed, it, got, exp);
			for (auto &q : p) printf("(%d,%d) ", q.x, q.y);
			puts(""); return 1;
		}
	}
	puts("OK manhattan-mst");
}
