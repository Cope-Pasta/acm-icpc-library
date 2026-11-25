#include "check/prelude.h"
#include "check/common/geo3d.h"
#include "code/computational-geometry/common-3d.cpp"

bool eq(double a, double b) { return fabs(a - b) <= 1e-6 * max(1.0, fabs(b)); }

int main() {
	unsigned seed = 1; mt19937 rng(seed);
	auto rnd = [&](int l, int r) { return int(rng() % (r - l + 1)) + l; };
	auto rp = [&]() { return point3_t(rnd(-1000, 1000) / 100.0, rnd(-1000, 1000) / 100.0, rnd(-1000, 1000) / 100.0); };
	for (int it = 0; it < 20000; ++it) {
		point3_t a = rp(), b = rp(), c = rp(), d = rp();
		auto fail = [&](const char *what, double got, double exp) {
			printf("seed %u it %d %s: got %.9f expected %.9f\n", seed, it, what, got, exp); return 1;
		};
		if (it % 4 == 0) b = a * (rnd(-3, 3) / 2.0); // parallel / zero vectors
		point3_t x = cross(a, b);
		if (!eq(dot(a, b), a.x * b.x + a.y * b.y + a.z * b.z)) return fail("dot", dot(a, b), 0);
		if (!eq(dot(x, a), 0) || !eq(dot(x, b), 0)) return fail("cross orthogonal", dot(x, a), 0);
		if (!eq(dot(x, x), dot(a, a) * dot(b, b) - dot(a, b) * dot(a, b))) return fail("cross length", dot(x, x), 0);
		double det = (b.x - a.x) * ((c.y - a.y) * (d.z - a.z) - (c.z - a.z) * (d.y - a.y))
			- (b.y - a.y) * ((c.x - a.x) * (d.z - a.z) - (c.z - a.z) * (d.x - a.x))
			+ (b.z - a.z) * ((c.x - a.x) * (d.y - a.y) - (c.y - a.y) * (d.x - a.x));
		if (!eq(volume(a, b, c, d), det / 6)) return fail("volume", volume(a, b, c, d), det / 6);
		double p = dist(a, b), q = dist(b, c), r = dist(c, a), s = (p + q + r) / 2; // Heron
		if (fabs(area(a, b, c) - sqrt(max(0.0, s * (s - p) * (s - q) * (s - r)))) > 1e-4) return fail("area", area(a, b, c), 0);
		// isFF: planes through a (normal b) and c (normal d)
		point3_t o1 = rp(), o2 = it % 5 == 0 ? o1 * 2 : rp();
		if (dot(cross(o1, o2), cross(o1, o2)) < 1e-3) {
			bool thrown = false;
			try { isFF(a, o1, c, o2); } catch (int) { thrown = true; }
			if (dblcmp(cross(o1, o2).length()) == 0 && !thrown) return fail("isFF parallel", 0, 0);
		} else {
			auto [u, v] = isFF(a, o1, c, o2);
			for (point3_t w : {u, v}) if (fabs(dot(w - a, o1)) > 1e-6 * o1.length() * max(1.0, w.length()) || fabs(dot(w - c, o2)) > 1e-6 * o2.length() * max(1.0, w.length()))
				return fail("isFF on planes", dot(w - a, o1), dot(w - c, o2));
			if (dblcmp((v - u).length()) == 0) return fail("isFF direction", 0, 0);
		}
		// distLL: lines a + s*u and c + t*v
		point3_t u = rp(), v = it % 3 == 0 ? u * (rnd(1, 4) / 2.0) : rp();
		if (u.length() < 0.1 || v.length() < 0.1) continue;
		point3_t n = cross(u, v); double exp;
		if (n.length() < 1e-9) { point3_t h = c - a; exp = (h - u * (dot(h, u) / dot(u, u))).length(); } // drop the u component
		else { // closest points a + s*u, c + t*v: (a + s*u - c - t*v) orthogonal to u and v
			point3_t w = a - c; double uu = dot(u, u), uv = dot(u, v), vv = dot(v, v), D = uu * vv - uv * uv;
			double s = (uv * dot(v, w) - vv * dot(u, w)) / D, t = (uu * dot(v, w) - uv * dot(u, w)) / D;
			exp = (a + u * s - c - v * t).length();
		}
		if (n.length() > 1e-9 && n.length() < 1e-2) continue; // nearly parallel: ill-conditioned
		if (fabs(distLL(a, u, c, v) - exp) > 1e-6 * max(1.0, exp)) return fail("distLL", distLL(a, u, c, v), exp);
	}
	puts("OK common-3d");
}
