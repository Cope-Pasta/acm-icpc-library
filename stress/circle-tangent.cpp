#include "check/prelude.h"
#include "check/common/geo2d.h"
struct seg_t { point_t a, b; seg_t(point_t a, point_t b) : a(a), b(b) {} };
#include "code/computational-geometry/circle-tangent.cpp"
const unsigned SEED = 9;
mt19937 rng(SEED);
int ri(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }
int fail(int it, const char *what) { printf("seed %u iter %d: %s\n", SEED, it, what); return 1; }

bool tangentAt(point_t c, double r, point_t q, point_t dir) { // q on circle, dir perpendicular to radius
	return fabs(dist(q, c) - r) < 1e-6 && fabs(dot(q - c, dir)) < 1e-6 * max(1.0, dist(dir));
}

int main() {
	for (int it = 0; it < 50000; ++it) {
		int R = ri(1, 10);
		point_t c(ri(-R, R), ri(-R, R)), p(ri(-R, R), ri(-R, R));
		int r = ri(1, R);
		LL d2 = (LL)dist2(c, p);
		vector<point_t> t = tanCP(c, r, p);
		if ((d2 < (LL)r * r) != t.empty()) return fail(it, "tanCP count");
		for (point_t q : t) if (!tangentAt(c, r, q, p - q) || (d2 == (LL)r * r && dist(q, p) > 1e-6)) return fail(it, "tanCP point");
		if (t.size() == 2 && d2 > (LL)r * r && dist(t[0], t[1]) < 1e-6) return fail(it, "tanCP points coincide");

		point_t c2(ri(-R, R), ri(-R, R));
		int r2 = ri(1, R);
		LL e2 = (LL)dist2(c, c2), out = (LL)(r + r2) * (r + r2), in = (LL)(r - r2) * (r - r2);
		if (e2 == 0 || e2 == out || e2 == in) continue; // concentric or tangent circles: tangent lines degenerate
		int want = e2 > out ? 4 : e2 > in ? 2 : 0;
		vector<seg_t> s = tanCC(c, r, c2, r2);
		if ((int)s.size() != want) return fail(it, "tanCC count");
		for (auto &g : s) if (!tangentAt(c, r, g.a, g.b - g.a) || !tangentAt(c2, r2, g.b, g.b - g.a)) return fail(it, "tanCC segment");
		for (int i = 0; i < want; ++i) for (int j = i + 1; j < want; ++j)
			if (dist(s[i].a, s[j].a) < 1e-6 && dist(s[i].b, s[j].b) < 1e-6) return fail(it, "tanCC duplicate line");
	}
	puts("OK circle-tangent");
}
