#include "check/prelude.h"
#include "check/common/geo2d.h"
#include "code/computational-geometry/common-2d.cpp"
#include "code/computational-geometry/minimum-circle.cpp"

int main() {
	unsigned seed = 1; mt19937 rng(seed);
	auto rnd = [&](int l, int r) { return int(rng() % (r - l + 1)) + l; };
	for (int it = 0; it < 3000; ++it) {
		int n = rnd(1, 12), R = rnd(0, 20);
		vector<point_t> p(n);
		for (auto &q : p) q = point_t(rnd(-R, R), rnd(-R, R));
		auto [c, r] = minCircle(p);
		double best = n == 1 ? 0 : 1e18; // O(n^4): every circle through 2 or 3 points
		auto chk = [&](point_t cc, double rr) { for (auto &q : p) if (dist(q, cc) > rr + 1e-7) return; best = min(best, rr); };
		for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) {
			chk((p[i] + p[j]) / 2, dist(p[i], p[j]) / 2);
			for (int k = j + 1; k < n; ++k) if (dblcmp(cross(p[j] - p[i], p[k] - p[i]))) {
				point_t cc = circumcenter(p[i], p[j], p[k]); chk(cc, dist(cc, p[i]));
			}
		}
		bool covers = true;
		for (auto &q : p) if (dist(q, c) > r + 1e-6) covers = false;
		if (!covers || fabs(best - r) > 1e-6) {
			printf("seed %u it %d: r=%.9f brute=%.9f covers=%d\n", seed, it, r, best, covers);
			for (auto &q : p) printf("(%g,%g) ", q.x, q.y);
			puts(""); return 1;
		}
	}
	puts("OK minimum-circle");
}
