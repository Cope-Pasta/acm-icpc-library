#include "check/prelude.h"
#include "check/common/geo2d.h"
#include "code/computational-geometry/circle-intersection.cpp"
const unsigned SEED = 8;
mt19937 rng(SEED);
int ri(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }
int fail(int it, const char *what) { printf("seed %u iter %d: %s\n", SEED, it, what); return 1; }

int main() {
	for (int it = 0; it < 50000; ++it) {
		int R = ri(1, 10);
		point_t c1(ri(-R, R), ri(-R, R)), c2(ri(-R, R), ri(-R, R));
		int r1 = ri(1, R), r2 = ri(1, R);
		LL d2 = (LL)dist2(c1, c2); // exact integer classification
		int want = d2 >= (LL)(r1 + r2) * (r1 + r2) ? 1 : r1 <= r2 && d2 <= (LL)(r2 - r1) * (r2 - r1) ? 2 : d2 <= (LL)(r1 - r2) * (r1 - r2) ? 3 : 0;
		if (testCC(c1, c2, r1, r2) != want) return fail(it, "testCC");
		if (want == 0 || (d2 > 0 && (d2 == (LL)(r1 + r2) * (r1 + r2) || d2 == (LL)(r1 - r2) * (r1 - r2)))) { // intersecting or tangent
			auto pr = isCC(c1, c2, r1, r2);
			for (point_t p : {pr.first, pr.second})
				if (fabs(dist(p, c1) - r1) > 1e-6 || fabs(dist(p, c2) - r2) > 1e-6) return fail(it, "isCC point not on both circles");
			if (want == 0 && dist(pr.first, pr.second) < 1e-6) return fail(it, "isCC points coincide");
		}
		// ray a->b against circle (c1, r1)
		point_t a(ri(-R, R), ri(-R, R)), b(ri(-R, R), ri(-R, R));
		if (a.x == b.x && a.y == b.y) continue;
		point_t u = (b - a) / dist(a, b);
		double tc = dot(c1 - a, u), h = fabs(cross(u, c1 - a));
		bool inside = dist(a, c1) <= r1, hits = inside || (h <= r1 + 1e-9 && tc >= 0);
		if (!hits || fabs(h - r1) < 1e-6 || fabs(dist(a, c1) - r1) < 1e-6) continue; // skip grazing / boundary cases
		double want_t = inside ? tc + sqrt(r1 * r1 - h * h) : tc - sqrt(r1 * r1 - h * h); // first hit along the ray
		point_t p = isCL(a, b, c1, r1);
		if (dist(p, a + u * want_t) > 1e-6) return fail(it, "isCL");
	}
	puts("OK circle-intersection");
}
