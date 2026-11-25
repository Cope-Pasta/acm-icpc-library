#include "check/prelude.h"
#include "check/common/geo2d.h"
namespace ci {
#include "code/computational-geometry/circle-intersection.cpp"
}
using ci::testCC;
#include "code/computational-geometry/circle-area-union.cpp"

double lens(double d, double r1, double r2) { // exact area of intersection of two circles
	if (d >= r1 + r2) return 0;
	if (d <= fabs(r1 - r2)) return pi * sqr(min(r1, r2));
	double a1 = acos((d * d + r1 * r1 - r2 * r2) / (2 * d * r1)), a2 = acos((d * d + r2 * r2 - r1 * r1) / (2 * d * r2));
	return r1 * r1 * (a1 - sin(2 * a1) / 2) + r2 * r2 * (a2 - sin(2 * a2) / 2);
}

int main() {
	unsigned seed = 1; mt19937 rng(seed);
	auto rnd = [&](int l, int r) { return int(rng() % (r - l + 1)) + l; };
	for (int it = 0; it < 600; ++it) {
		int n = it < 300 ? rnd(1, 2) : rnd(1, 7);
		vector<point_t> c; vector<double> r;
		while ((int)c.size() < n) { // no coincided circles
			point_t p(rnd(-10, 10) + rnd(0, 99) / 100.0, rnd(-10, 10) + rnd(0, 99) / 100.0);
			double rr = rnd(1, 80) / 10.0; bool dup = false;
			for (int i = 0; i < (int)c.size(); ++i) if (dist(c[i], p) < 1e-9 && fabs(r[i] - rr) < 1e-9) dup = true;
			if (!dup) c.push_back(p), r.push_back(rr);
		}
		auto a = circle_union(c, r);
		auto fail = [&](const char *what, int k, double got, double exp) {
			printf("seed %u it %d %s k=%d: got %.9f expected %.9f\n", seed, it, what, k, got, exp);
			for (int i = 0; i < n; ++i) printf("(%g,%g,r=%g) ", c[i].x, c[i].y, r[i]);
			puts(""); return 1;
		};
		if ((int)a.size() != n + 1) return fail("size", 0, a.size(), n + 1);
		double weighted = 0, total = 0; // sum k * area[k] == sum of circle areas
		for (int k = 1; k <= n; ++k) weighted += k * a[k];
		for (int i = 0; i < n; ++i) total += pi * r[i] * r[i];
		if (fabs(weighted - total) > 1e-6 * max(1.0, total)) return fail("sum k*area", 0, weighted, total);
		if (n == 2) {
			double in = lens(dist(c[0], c[1]), r[0], r[1]);
			if (fabs(a[2] - in) > 1e-6 || fabs(a[1] - (total - 2 * in)) > 1e-6) return fail("lens", 2, a[2], in);
		}
		if (it % 10 == 0) { // grid estimate of each exact-k area
			const int G = 800; const double L = -20, H = 20, s = (H - L) / G;
			vector<double> g(n + 1);
			for (int x = 0; x < G; ++x) for (int y = 0; y < G; ++y) {
				point_t p(L + (x + .5) * s, L + (y + .5) * s); int k = 0;
				for (int i = 0; i < n; ++i) k += dist(p, c[i]) < r[i];
				g[k] += s * s;
			}
			for (int k = 1; k <= n; ++k) if (fabs(g[k] - a[k]) > 0.01 * max(1.0, a[k]) + 0.3) return fail("grid", k, a[k], g[k]);
		}
	}
	puts("OK circle-area-union");
}
