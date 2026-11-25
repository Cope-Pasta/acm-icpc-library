#include "check/prelude.h"
#include "check/common/geo2d.h"
typedef pair<point_t, point_t> seg_t;
#include "code/computational-geometry/circle-tangent.cpp"
#include "stress/test.h"
const unsigned SEED = 9;
mt19937 rng(SEED);
int ri(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }
int fail(int it, const char *what) { printf("seed %u iter %d: %s\n", SEED, it, what); return 1; }

bool tangentAt(point_t c, double r, point_t q, point_t dir) { // q on circle, dir perpendicular to radius
	return fabs(dist(q, c) - r) < 1e-6 && fabs(dot(q - c, dir)) < 1e-6 * max(1.0, dist(dir));
}


int unit() {
	auto touches = [](seg_t s, point_t c, double r) { point_t d = s.second - s.first; return near(fabs(cross(d, c - s.first)) / dist(d), r); };
	EXPECT(tanCC({0, 0}, 1, {0, 0}, 1).empty()); // same circle
	EXPECT(tanCC({0, 0}, 1, {0, 0}, 2).empty()); // concentric
	EXPECT(tanCC({0, 0}, 3, {1, 0}, 1).empty()); // nested
	struct { point_t c1; double r1; point_t c2; double r2; int lines; } cs[] = {
		{{0, 0}, 1, {2, 0}, 1, 3}, {{0, 0}, 1, {3, 0}, 2, 3}, // externally touching
		{{0, 0}, 2, {1, 0}, 1, 1}, {{0, 0}, 1, {1, 0}, 2, 1}, // internally touching
		{{0, 0}, 1, {5, 0}, 1, 4}, {{0, 0}, 1, {1, 0}, 1, 2}, // equal radii: apart, intersecting
		{{0.1, 0.2}, 0.1, {0.4, 0.2}, 0.2, 3}}; // touching, non-integer
	for (auto &t : cs) {
		vector<seg_t> s = tanCC(t.c1, t.r1, t.c2, t.r2);
		EXPECT((int)s.size() == t.lines);
		for (auto &g : s) EXPECT(dblcmp(dist(g.first, g.second)) > 0 && near(dist(g.first, t.c1), t.r1) && touches(g, t.c1, t.r1) && touches(g, t.c2, t.r2));
	}
	vector<seg_t> s = tanCC({0, 0}, 1, {2, 0}, 1);
	EXPECT(near(s[2].first.x, 1) && near(s[2].first.y, 0) && near(s[2].second.x, 1)); // common tangent x = 1 at the touching point
	vector<point_t> t = tanCP({0, 0}, 1, {1, 0});
	EXPECT(t.size() == 1 && near(t[0].x, 1) && near(t[0].y, 0)); // point on the circle
	EXPECT(tanCP({0, 0}, 2, {1, 0}).empty()); // point inside
	t = tanCP({0, 0}, 1, {2, 0});
	EXPECT(t.size() == 2 && near(t[0].x, 0.5) && near(t[1].x, 0.5) && near(fabs(t[0].y), sqrt(3) / 2) && near(t[0].y, -t[1].y));
	return 0;
}

int main() {
	if (unit()) return 1;
	for (int it = 0; it < 50000; ++it) {
		int R = ri(1, 10);
		point_t c(ri(-R, R), ri(-R, R)), p(ri(-R, R), ri(-R, R));
		int r = ri(1, R);
		ll d2 = (ll)dist2(c, p);
		vector<point_t> t = tanCP(c, r, p);
		if ((d2 < (ll)r * r) != t.empty()) return fail(it, "tanCP count");
		for (point_t q : t) if (!tangentAt(c, r, q, p - q) || (d2 == (ll)r * r && dist(q, p) > 1e-6)) return fail(it, "tanCP point");
		if (t.size() == 2 && d2 > (ll)r * r && dist(t[0], t[1]) < 1e-6) return fail(it, "tanCP points coincide");

		point_t c2(ri(-R, R), ri(-R, R));
		int r2 = ri(1, R);
		ll e2 = (ll)dist2(c, c2), out = (ll)(r + r2) * (r + r2), in = (ll)(r - r2) * (r - r2);
		if (e2 == 0 || e2 == out || e2 == in) continue; // concentric or tangent circles: tangent lines degenerate
		int want = e2 > out ? 4 : e2 > in ? 2 : 0;
		vector<seg_t> s = tanCC(c, r, c2, r2);
		if ((int)s.size() != want) return fail(it, "tanCC count");
		for (auto &g : s) if (!tangentAt(c, r, g.first, g.second - g.first) || !tangentAt(c2, r2, g.second, g.second - g.first)) return fail(it, "tanCC segment");
		for (int i = 0; i < want; ++i) for (int j = i + 1; j < want; ++j)
			if (dist(s[i].first, s[j].first) < 1e-6 && dist(s[i].second, s[j].second) < 1e-6) return fail(it, "tanCC duplicate line");
	}
	puts("OK circle-tangent");
}
