#include "check/prelude.h"
#include "check/common/geo2d.h"
#include "code/computational-geometry/closest-pair-points.cpp"
#include "stress/test.h"
const unsigned SEED = 4;
mt19937 rng(SEED);
int ri(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }


int unit() {
	EXPECT(near(closestPair({{1, 1}, {1, 1}}), 0)); // duplicates
	EXPECT(near(closestPair({{0, 0}, {3, 4}}), 5));
	EXPECT(near(closestPair({{0, 0}, {5, 5}, {9, 9}, {0, 0}, {7, 1}}), 0));
	EXPECT(near(closestPair({{0, 0}, {0, 10}, {0, 3}, {0, 7}, {0, 12}, {0, 20}, {0, 30}, {0, 41}}), 2)); // all on one vertical line
	EXPECT(near(closestPair({{0, 5}, {10, 5}, {3, 5}, {7, 5}, {12, 5}, {20, 5}, {30, 5}, {41, 5}}), 2)); // all on one horizontal line
	return 0;
}

int main() {
	if (unit()) return 1;
	for (int it = 0; it < 20000; ++it) {
		int n = ri(2, it % 100 == 0 ? 300 : 40), R = ri(1, 1000);
		vector<point_t> s(n);
		for (auto &q : s) q = point_t(ri(-R, R), ri(-R, R));
		double best = 1e100;
		for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) best = min(best, dist(s[i], s[j]));
		double got = closestPair(s);
		if (dblcmp(got - best)) {
			printf("seed %u iter %d: closest %.9f, brute %.9f\n", SEED, it, got, best);
			for (auto &q : s) printf("(%g,%g) ", q.x, q.y);
			puts("");
			return 1;
		}
	}
	puts("OK closest-pair-points");
}
