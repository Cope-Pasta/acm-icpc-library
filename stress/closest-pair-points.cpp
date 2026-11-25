#include "check/prelude.h"
#include "check/common/geo2d.h"
bool cmpX(const point_t &a, const point_t &b) { return a.x < b.x; }
bool cmpY(const point_t &a, const point_t &b) { return a.y < b.y; }
#include "code/computational-geometry/closest-pair-points.cpp"
const unsigned SEED = 4;
mt19937 rng(SEED);
int ri(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }

int main() {
	for (int it = 0; it < 20000; ++it) {
		int n = ri(2, it % 100 == 0 ? 300 : 40), R = ri(1, 1000);
		vector<point_t> s(n);
		for (auto &q : s) q = point_t(ri(-R, R), ri(-R, R));
		double best = 1e100;
		for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) best = min(best, dist(s[i], s[j]));
		double got = cal(s);
		if (dblcmp(got - best)) {
			printf("seed %u iter %d: closest %.9f, brute %.9f\n", SEED, it, got, best);
			for (auto &q : s) printf("(%g,%g) ", q.x, q.y);
			puts("");
			return 1;
		}
	}
	puts("OK closest-pair-points");
}
