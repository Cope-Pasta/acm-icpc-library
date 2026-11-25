#include "check/prelude.h"
#include "check/common/geo2d.h"
#include "code/computational-geometry/convex-hull-2d.cpp"
#include "code/computational-geometry/rotating-caliper.cpp"
#include "stress/test.h"
const unsigned SEED = 2;
mt19937 rng(SEED);
int ri(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }


int unit() {
	EXPECT(near(rotate_caliper({{1, 1}}), 0)); // single point
	EXPECT(near(rotate_caliper({{0, 0}, {3, 4}}), 5)); // segment
	EXPECT(near(rotate_caliper(graham({{0, 0}, {2, 0}, {2, 2}, {0, 2}, {1, 1}})), sqrt(8.0)));
	EXPECT(near(rotate_caliper(graham({{0, 0}, {1, 0}, {2, 0}, {10, 0}})), 10)); // collinear input
	return 0;
}

int main() {
	if (unit()) return 1;
	for (int it = 0; it < 20000; ++it) {
		int n = ri(2, 15), R = ri(1, 20);
		vector<point_t> s(n);
		for (auto &q : s) q = point_t(ri(-R, R), ri(-R, R));
		vector<point_t> h = graham(s);
		if (h.size() < 3) continue;
		double best = 0;
		for (auto &a : s) for (auto &b : s) best = max(best, dist(a, b));
		double got = rotate_caliper(h);
		if (dblcmp(got - best)) {
			printf("seed %u iter %d: caliper %.9f, brute %.9f\n", SEED, it, got, best);
			for (auto &q : s) printf("(%g,%g) ", q.x, q.y);
			puts("");
			return 1;
		}
	}
	puts("OK rotating-caliper");
}
