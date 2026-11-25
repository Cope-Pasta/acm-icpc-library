#include "check/prelude.h"
#include "check/common/geo2d.h"
#include "code/computational-geometry/common-2d.cpp"
#include "stress/test.h"
#include <sys/wait.h>
#include <unistd.h>
const unsigned SEED = 5;
mt19937 rng(SEED);
int ri(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }
point_t rp(int R) { return point_t(ri(-R, R), ri(-R, R)); }
int fail(int it, const char *what) { printf("seed %u iter %d: %s\n", SEED, it, what); return 1; }

int sgn(double x) { return (x > 0) - (x < 0); } // exact on small integers
bool onSegB(point_t p, point_t a, point_t b) {
	return sgn(cross(a - p, b - p)) == 0 && min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) && min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}
int testSSB(point_t a, point_t b, point_t c, point_t d) {
	int d1 = sgn(cross(b - a, c - a)), d2 = sgn(cross(b - a, d - a)), d3 = sgn(cross(d - c, a - c)), d4 = sgn(cross(d - c, b - c));
	if (d1 * d2 < 0 && d3 * d4 < 0) return 1;
	if (onSegB(c, a, b) || onSegB(d, a, b) || onSegB(a, c, d) || onSegB(b, c, d)) return -1;
	return 0;
}
bool insideB(const vector<point_t> &p, point_t t) { // boundary counts as inside, crossing number otherwise
	int n = p.size(); bool in = false;
	for (int i = 0; i < n; ++i) {
		point_t a = p[i], b = p[(i + 1) % n];
		if (onSegB(t, a, b)) return true;
		if ((a.y > t.y) != (b.y > t.y) && t.x < a.x + (b.x - a.x) * (t.y - a.y) / (b.y - a.y)) in = !in;
	}
	return in;
}
double lineDist(point_t p, point_t a, point_t b) { return fabs(cross(b - a, p - a)) / dist(a, b); }


int unit() {
	EXPECT(isLL({0, 0}, {1, 0}, {0, 1}, {1, 1}).empty()); // parallel
	EXPECT(isLL({0, 0}, {1, 0}, {2, 0}, {3, 0}).empty()); // same line
	vector<point_t> x = isLL({0, 0}, {2, 2}, {0, 2}, {2, 0}); EXPECT(x.size() == 1 && near(x[0].x, 1) && near(x[0].y, 1));
	EXPECT(testSS({0, 0}, {2, 2}, {0, 2}, {2, 0}) == 1);
	EXPECT(testSS({0, 0}, {2, 0}, {1, 0}, {3, 0}) == -1); // collinear overlap
	EXPECT(testSS({0, 0}, {1, 0}, {1, 0}, {2, 0}) == -1); // shared endpoint
	EXPECT(testSS({0, 0}, {2, 0}, {1, 0}, {1, 5}) == -1); // T-junction
	EXPECT(testSS({0, 0}, {1, 0}, {2, 0}, {3, 0}) == 0); // collinear, disjoint
	EXPECT(testSS({0, 0}, {1, 1}, {3, 0}, {2, 1}) == 0);
	vector<point_t> sq = {{0, 0}, {2, 0}, {2, 2}, {0, 2}};
	for (point_t t : vector<point_t>{{0, 0}, {2, 2}, {1, 0}, {0, 1}, {1, 1}}) EXPECT(insideConvex(sq, t) && insidePoly(sq, t)); // vertices, edges, interior
	for (point_t t : vector<point_t>{{3, 1}, {-1e-3, 1}, {1, 2.001}, {3, 3}, {-1, -1}}) EXPECT(!insideConvex(sq, t) && !insidePoly(sq, t));
	EXPECT(insideConvex({{1, 1}}, {1, 1}) && !insideConvex({{1, 1}}, {1, 2})); // single point
	EXPECT(insideConvex({{0, 0}, {2, 2}}, {1, 1}) && !insideConvex({{0, 0}, {2, 2}}, {3, 3}) && !insideConvex({{0, 0}, {2, 2}}, {1, 0})); // segment
	auto aborts = [](function<void()> f) { // f() hits an assert
		pid_t pid = fork();
		if (pid == 0) { freopen("/dev/null", "w", stderr); f(); _exit(0); }
		int st; waitpid(pid, &st, 0); return WIFSIGNALED(st);
	};
	EXPECT(aborts([] { circumcenter({0, 0}, {1, 1}, {2, 2}); }) && aborts([] { circumcenter({0, 0}, {0, 0}, {1, 0}); }));
	EXPECT(aborts([] { incenter({0, 0}, {1, 1}, {2, 2}); }));
	EXPECT(!aborts([] { circumcenter({0, 0}, {1, 0}, {0, 1}); incenter({0, 0}, {1, 0}, {0, 1}); }));
	vector<point_t> u = {{0, 0}, {3, 0}, {3, 3}, {2, 3}, {2, 1}, {1, 1}, {1, 3}, {0, 3}}; // U shape
	EXPECT(insidePoly(u, {0.5, 2}) && !insidePoly(u, {1.5, 2}) && insidePoly(u, {1.5, 1}) && insidePoly(u, {1.5, 0.5}) && insidePoly(u, {2, 2}));
	return 0;
}

int main() {
	if (unit()) return 1;
	for (int it = 0; it < 20000; ++it) {
		int R = ri(1, 10);
		point_t a = rp(R), b = rp(R), c = rp(R), d = rp(R);
		if (onSeg(a, c, d) != onSegB(a, c, d)) return fail(it, "onSeg");
		if (testSS(a, b, c, d) != testSSB(a, b, c, d)) return fail(it, "testSS");
		vector<point_t> x = isLL(a, b, c, d);
		bool par = sgn(cross(b - a, d - c)) == 0;
		if (!(a.x == b.x && a.y == b.y) && !(c.x == d.x && c.y == d.y)) {
			if (par != x.empty()) return fail(it, "isLL parallel");
			if (!par && (lineDist(x[0], a, b) > 1e-6 || lineDist(x[0], c, d) > 1e-6)) return fail(it, "isLL point");
		}
		if (fabs(cross(b - a, c - a)) > 0.5) { // non-degenerate triangle a, b, c
			point_t o = circumcenter(a, b, c), i = incenter(a, b, c), h = orthocenter(a, b, c);
			if (fabs(dist(o, a) - dist(o, b)) > 1e-6 || fabs(dist(o, a) - dist(o, c)) > 1e-6) return fail(it, "circumcenter");
			double r = lineDist(i, a, b);
			if (fabs(r - lineDist(i, b, c)) > 1e-6 || fabs(r - lineDist(i, c, a)) > 1e-6 || !insideB({a, b, c}, i) && !insideB({a, c, b}, i)) return fail(it, "incenter");
			if (fabs(dot(h - a, b - c)) > 1e-6 || fabs(dot(h - b, c - a)) > 1e-6) return fail(it, "orthocenter");
		}
		double ang = (rng() % 10000) / 10000.0 * 2 * pi - pi;
		point_t q = rotate(a, ang);
		if (fabs(dist(q) - dist(a)) > 1e-9 || dist(rotate(q, -ang), a) > 1e-9) return fail(it, "rotate");
		if (dist(a) > 0 && fabs(angle(a, q) - ang) > 1e-9 && fabs(fabs(angle(a, q) - ang) - 2 * pi) > 1e-9) return fail(it, "angle");

		// star-shaped simple polygon around a non-lattice center
		int n = ri(3, 10);
		vector<point_t> poly(n);
		for (auto &p : poly) p = rp(R + 2);
		point_t ct(0.123, 0.0457);
		sort(poly.begin(), poly.end(), [&](point_t u, point_t v) { return atan2(u.y - ct.y, u.x - ct.x) < atan2(v.y - ct.y, v.x - ct.x); });
		poly.erase(unique(poly.begin(), poly.end(), [](point_t u, point_t v) { return u.x == v.x && u.y == v.y; }), poly.end());
		for (int k = 0; k < 20 && poly.size() >= 3; ++k) {
			point_t t(ri(-2 * R - 6, 2 * R + 6) / 2.0, ri(-2 * R - 6, 2 * R + 6) / 2.0);
			if (insidePoly(poly, t) != insideB(poly, t)) return fail(it, "insidePoly");
		}
		// convex polygon: CCW, no three collinear
		vector<point_t> hull;
		{
			vector<point_t> s = poly;
			sort(s.begin(), s.end(), [](point_t u, point_t v) { return u.x < v.x || (u.x == v.x && u.y < v.y); });
			for (int pass = 0; pass < 2; ++pass) {
				size_t st = hull.size();
				for (auto &p : s) {
					while (hull.size() >= st + 2 && cross(hull.back() - hull[hull.size() - 2], p - hull[hull.size() - 2]) <= 0) hull.pop_back();
					hull.push_back(p);
				}
				hull.pop_back();
				reverse(s.begin(), s.end());
			}
		}
		if (hull.size() < 3) continue;
		for (int k = 0; k < 20; ++k) {
			point_t t(ri(-2 * R - 6, 2 * R + 6) / 2.0, ri(-2 * R - 6, 2 * R + 6) / 2.0);
			if (insideConvex(hull, t) != insideB(hull, t)) return fail(it, "insideConvex");
		}
	}
	puts("OK common-2d");
}
