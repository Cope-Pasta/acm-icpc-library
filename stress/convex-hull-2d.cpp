#include "check/prelude.h"
#include "check/common/geo2d.h"
#include "code/computational-geometry/convex-hull-2d.cpp"
const unsigned SEED = 1;
mt19937 rng(SEED);
int ri(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }

bool extreme(const vector<point_t> &s, point_t q) { // q is a strict hull vertex: some open half-plane gap > pi
	vector<double> ang;
	for (auto &r : s) if (dblcmp(r.x - q.x) || dblcmp(r.y - q.y)) ang.push_back(atan2(r.y - q.y, r.x - q.x));
	if (ang.empty()) return true;
	sort(ang.begin(), ang.end());
	double gap = ang[0] + 2 * pi - ang.back();
	for (size_t i = 1; i < ang.size(); ++i) gap = max(gap, ang[i] - ang[i - 1]);
	return gap > pi + 1e-9;
}

int main() {
	for (int it = 0; it < 20000; ++it) {
		int n = ri(1, 12), R = ri(1, 15);
		vector<point_t> s(n);
		for (auto &q : s) q = point_t(ri(-R, R), ri(-R, R));
		if (it % 10 == 0) for (auto &q : s) q = point_t(q.x, 2 * q.x + 1); // all collinear
		vector<point_t> h = graham(s);
		set<pair<double, double>> bv, hv;
		for (auto &q : s) if (extreme(s, q)) bv.insert({q.x, q.y});
		for (auto &q : h) hv.insert({q.x, q.y});
		bool bad = false;
		if (bv.size() >= 3) {
			bad = bv != hv || hv.size() != h.size();
			int m = h.size();
			for (int i = 0; i < m; ++i) if (cross(h[(i + 1) % m] - h[i], h[(i + 2) % m] - h[i]) <= 0) bad = true; // strictly CCW
			for (int i = 1; i < m; ++i) if (cmp(h[i], h[0])) bad = true; // starts at lowest point
		} else bad = h.size() > (size_t)n || h.empty() != s.empty();
		if (bad) {
			printf("seed %u iter %d: graham mismatch\n", SEED, it);
			for (auto &q : s) printf("(%g,%g) ", q.x, q.y);
			puts("");
			return 1;
		}
	}
	puts("OK convex-hull-2d");
}
