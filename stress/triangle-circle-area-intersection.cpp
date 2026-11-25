#include "check/prelude.h"
#include "check/common/geo2d.h"
#include "code/computational-geometry/triangle-circle-area-intersection.cpp"
const unsigned SEED = 10;
mt19937 rng(SEED);
double rd(double a, double b) { return uniform_real_distribution<double>(a, b)(rng); }

double brute(point_t ct, double r, vector<point_t> tri) { // clip a 4096-gon approximating the circle by the CCW triangle
	vector<point_t> poly;
	const int N = 4096;
	for (int i = 0; i < N; ++i) poly.push_back(ct + point_t(cos(2 * pi * i / N), sin(2 * pi * i / N)) * r);
	for (int i = 0; i < 3; ++i) {
		point_t a = tri[i], b = tri[(i + 1) % 3];
		vector<point_t> nxt;
		for (size_t j = 0; j < poly.size(); ++j) {
			point_t p = poly[j], q = poly[(j + 1) % poly.size()];
			double cp = cross(b - a, p - a), cq = cross(b - a, q - a);
			if (cp >= 0) nxt.push_back(p);
			if ((cp >= 0) != (cq >= 0)) nxt.push_back(p + (q - p) * (cp / (cp - cq)));
		}
		poly = nxt;
	}
	double s = 0;
	for (size_t j = 0; j < poly.size(); ++j) s += cross(poly[j], poly[(j + 1) % poly.size()]);
	return s / 2;
}

int main() {
	for (int it = 0; it < 6000; ++it) {
		bool in = it % 2; // odd: integer coordinates, hits vertices on the circle and right angles at the center
		auto g = [&](double a, double b) { return in ? floor(rd(a, b + 1)) : rd(a, b); };
		double R = g(1, 10), r = in ? g(1, R) : rd(0.5, R);
		point_t ct(g(-R, R), g(-R, R));
		vector<point_t> t(3);
		for (auto &q : t) q = point_t(g(-R, R), g(-R, R));
		double s = cross(t[1] - t[0], t[2] - t[0]);
		if (fabs(s) < 1e-3) continue;
		double got = areaTC(ct, r, t[0], t[1], t[2]);
		if (s < 0) swap(t[1], t[2]);
		double want = brute(ct, r, t) * (s < 0 ? -1 : 1);
		if (fabs(got - want) > 1e-5 * r * r + 1e-9 || std::isnan(got)) {
			printf("seed %u iter %d: areaTC %.9f, brute %.9f\n", SEED, it, got, want);
			printf("c=(%.6f,%.6f) r=%.6f tri=(%.6f,%.6f) (%.6f,%.6f) (%.6f,%.6f)\n", ct.x, ct.y, r, t[0].x, t[0].y, t[1].x, t[1].y, t[2].x, t[2].y);
			return 1;
		}
	}
	puts("OK triangle-circle-area-intersection");
}
