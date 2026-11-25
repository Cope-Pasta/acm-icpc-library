#include "check/prelude.h"
#include "check/common/geo2d.h"
#include "code/computational-geometry/convex-hull-2d.cpp"
#include "code/computational-geometry/polygon-area-union.cpp"

double area(const vector<point_t> &p) {
	double s = 0;
	for (int i = 0; i < (int)p.size(); ++i) s += cross(p[i], p[(i + 1) % p.size()]);
	return s / 2;
}

vector<point_t> clip(const vector<point_t> &p, point_t a, point_t b) { // keep left of a->b
	vector<point_t> r;
	for (int i = 0; i < (int)p.size(); ++i) {
		point_t u = p[i], v = p[(i + 1) % p.size()];
		double cu = cross(b - a, u - a), cv = cross(b - a, v - a);
		if (cu >= 0) r.push_back(u);
		if ((cu > 0 && cv < 0) || (cu < 0 && cv > 0)) r.push_back(u + (v - u) * (cu / (cu - cv)));
	}
	return r;
}

double brute(vector<vector<point_t>> ps) { // inclusion-exclusion over convex intersections
	int n = ps.size(); double res = 0;
	for (auto &p : ps) if (area(p) < 0) reverse(p.begin(), p.end());
	for (int m = 1; m < (1 << n); ++m) {
		vector<point_t> cur; bool first = true;
		for (int i = 0; i < n; ++i) if (m >> i & 1) {
			if (first) { cur = ps[i]; first = false; continue; }
			for (int j = 0; j < (int)ps[i].size() && !cur.empty(); ++j) cur = clip(cur, ps[i][j], ps[i][(j + 1) % ps[i].size()]);
		}
		res += (__builtin_popcount(m) & 1 ? 1 : -1) * (cur.size() < 3 ? 0 : area(cur));
	}
	return res;
}

int main() {
	unsigned seed = 1; mt19937 rng(seed);
	auto rnd = [&](int l, int r) { return int(rng() % (r - l + 1)) + l; };
	for (int it = 0; it < 3000; ++it) {
		int n = rnd(1, 5); bool frac = it % 2;
		vector<vector<point_t>> ps;
		while ((int)ps.size() < n) {
			if (!ps.empty() && rnd(0, 4) == 0) { ps.push_back(ps.back()); continue; } // identical polygons
			vector<point_t> pts(rnd(3, 8));
			for (auto &q : pts) q = frac ? point_t(rnd(-1000, 1000) / 97.0, rnd(-1000, 1000) / 89.0) : point_t(rnd(-10, 10), rnd(-10, 10));
			auto h = graham(pts);
			if (h.size() >= 3) {
				if (rnd(0, 1)) reverse(h.begin(), h.end()); // either orientation
				ps.push_back(h);
			}
		}
		double got = polyUnion(ps), exp = brute(ps);
		if (fabs(got - exp) > 1e-6 * max(1.0, exp)) {
			printf("seed %u it %d: got %.9f expected %.9f\n", seed, it, got, exp);
			for (auto &p : ps) { for (auto &q : p) printf("(%g,%g) ", q.x, q.y); puts(""); }
			return 1;
		}
	}
	puts("OK polygon-area-union");
}
