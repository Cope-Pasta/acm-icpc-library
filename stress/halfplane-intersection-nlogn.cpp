#include "check/prelude.h"
#include "check/common/geo2d.h"
const double maxd = 1e5;
point_t isLL(point_t a, point_t b, point_t c, point_t d) { point_t u = b - a, v = d - c; return a + u * (cross(c - a, v) / cross(u, v)); }
#include "code/computational-geometry/halfplane-intersection-nlogn.cpp"
#include "stress/test.h"
const unsigned SEED = 7;
mt19937 rng(SEED);
int ri(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }


double area(const vector<point_t> &p) { double s = 0; for (int i = 0; i < (int)p.size(); ++i) s += cross(p[i], p[(i + 1) % p.size()]); return s / 2; }
vector<point_t> run(vector<pair<point_t, point_t>> hs) { init(); for (auto &h : hs) addhp(h.first, h.second); return hp_inter(); }
int unit() {
	EXPECT(run({{{1, 1}, {1, 0}}, {{-1, 0}, {-1, 1}}}).empty()); // x >= 1 and x <= -1
	EXPECT(run({{{0, 0}, {1, 0}}, {{1, 0}, {0, 1}}, {{0, 1}, {0, 0}}, {{1, 2}, {2, 2}}}).empty()); // triangle and y >= 2
	vector<point_t> p = run({{{0, 0}, {1, 0}}, {{1, 0}, {1, 1}}, {{1, 1}, {0, 1}}, {{0, 1}, {0, 0}}});
	EXPECT(p.size() == 4 && near(area(p), 1)); // unit square
	p = run({{{0, 0}, {1, 0}}, {{5, -1}, {6, -1}}, {{0, 0}, {1, 0}}, {{1, 0}, {1, 1}}, {{1, 1}, {0, 1}}, {{0, 1}, {0, 0}}});
	EXPECT(near(area(p), 1)); // duplicate and redundant half-planes
	p = run({{{0, 0}, {1, 0}}, {{1, 0}, {0, 1}}, {{0, 1}, {0, 0}}}); EXPECT(p.size() == 3 && near(area(p), 0.5));
	p = run({{{0, 0}, {0, 1}}, {{0, 1}, {0, 0}}}); EXPECT(near(area(p), 0)); // x = 0: degenerate
	p = run({}); EXPECT(near(area(p), 4 * maxd * maxd)); // just the bounding box
	return 0;
}

int main() {
	if (unit()) return 1;
	for (int it = 0; it < 5000; ++it) {
		int m = ri(1, 12), R = ri(2, 20);
		point_t q(ri(-R, R) + 0.37, ri(-R, R) + 0.61); // every half-plane contains q strictly
		vector<pair<point_t, point_t>> hs;
		while ((int)hs.size() < m) {
			point_t a(ri(-R, R), ri(-R, R)), b(ri(-R, R), ri(-R, R));
			double c = cross(b - a, q - a);
			if (dblcmp(dist(a, b)) == 0 || fabs(c) < 0.1) continue;
			if (c < 0) swap(a, b); // q on the left
			hs.push_back({a, b});
		}
		init();
		for (auto &h : hs) addhp(h.first, h.second);
		vector<point_t> p = hp_inter();
		int n = p.size();
		bool bad = n < 3;
		for (int i = 0; i < n && !bad; ++i) { // result vertices satisfy every half-plane, polygon is CCW
			for (auto &h : hs) if (cross(h.second - h.first, p[i] - h.first) < -1e-6) bad = true;
			if (cross(p[(i + 1) % n] - p[i], p[(i + 2) % n] - p[i]) < -1e-6) bad = true;
		}
		for (int k = 0; k < 200 && !bad; ++k) { // sampled points: inside polygon <=> inside all half-planes
			point_t t((int)(rng() % 20001) / 10000.0 * (R + 1) - (R + 1), (int)(rng() % 20001) / 10000.0 * (R + 1) - (R + 1));
			double mn = 1e18;
			for (auto &h : hs) mn = min(mn, cross(h.second - h.first, t - h.first) / dist(h.first, h.second));
			if (fabs(mn) < 1e-6) continue;
			bool in = true;
			for (int i = 0; i < n; ++i) if (cross(p[(i + 1) % n] - p[i], t - p[i]) < -1e-9) in = false; // edges may degenerate to a point
			if (in != (mn > 0)) bad = true;
		}
		if (bad) {
			printf("seed %u iter %d: halfplane intersection mismatch\n", SEED, it);
			for (auto &h : hs) printf("(%g,%g)->(%g,%g) ", h.first.x, h.first.y, h.second.x, h.second.y);
			puts("");
			return 1;
		}
	}
	puts("OK halfplane-intersection-nlogn");
}
