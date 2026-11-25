#include "check/prelude.h"
#include "check/common/geo2d.h"
#include "code/computational-geometry/convex-hull-2d.cpp"
#include "code/computational-geometry/minkowski-convex-hull.cpp"
const unsigned SEED = 3;
mt19937 rng(SEED);
int ri(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }

vector<point_t> randHull(int R) {
	vector<point_t> s(ri(1, 10));
	for (auto &q : s) q = point_t(ri(-R, R), ri(-R, R));
	return convexHull(s);
}

int main() {
	for (int it = 0; it < 20000; ++it) {
		int R = ri(2, 20);
		vector<point_t> a = randHull(R), b = randHull(R);
		if (it % 2 && (a.size() < 3 || b.size() < 3)) continue;
		vector<point_t> sums;
		for (auto &p : a) for (auto &q : b) sums.push_back(p + q);
		vector<point_t> want = convexHull(sums), got = minkowski(a, b);
		bool ok = want.size() == got.size();
		for (size_t i = 0; ok && i < got.size(); ++i) ok = !dblcmp(want[i].x - got[i].x) && !dblcmp(want[i].y - got[i].y);
		if (!ok) {
			printf("seed %u iter %d: minkowski size %zu, brute %zu\n", SEED, it, got.size(), want.size());
			for (auto &q : a) printf("(%g,%g) ", q.x, q.y);
			puts("");
			for (auto &q : b) printf("(%g,%g) ", q.x, q.y);
			puts("");
			return 1;
		}
	}
	puts("OK minkowski-convex-hull");
}
