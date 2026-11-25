#include "check/prelude.h"
#include "code/sap-maximum-flow.cpp"
// brute: max flow = min s-t cut over all vertex subsets
mt19937 rng(1); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
int main() {
	for (int it = 0; it < 20000; ++it) {
		int n = rnd(2, 8), m = rnd(0, 20), s = rnd(0, n - 1), t = rnd(0, n - 2); if (t >= s) ++t;
		vector<array<int, 3>> es(m);
		for (auto &[u, v, r] : es) u = rnd(0, n - 1), v = rnd(0, n - 1), r = rnd(0, 10);
		SAP g(n); for (auto [u, v, r] : es) g.add(u, v, r);
		int best = INT_MAX;
		for (int S = 0; S < 1 << n; ++S) if ((S >> s & 1) && !(S >> t & 1)) {
			int c = 0; for (auto [u, v, r] : es) if ((S >> u & 1) && !(S >> v & 1)) c += r;
			best = min(best, c);
		}
		int f = g.sap(s, t);
		if (f != best) { printf("FAIL it=%d n=%d s=%d t=%d: sap %d cut %d\n", it, n, s, t, f, best); for (auto [u, v, r] : es) printf("%d %d %d\n", u, v, r); return 1; }
	}
	puts("OK sap-maximum-flow");
}
