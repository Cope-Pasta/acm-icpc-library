#include "check/prelude.h"
#include "check/common/geo3d.h"
#include "code/computational-geometry/common-3d.cpp"
#include "code/computational-geometry/convex-hull-3d.cpp"

int main() {
	unsigned seed = 1; mt19937 rng(seed);
	auto rnd = [&](int l, int r) { return int(rng() % (r - l + 1)) + l; };
	for (int it = 0; it < 1500; ++it) {
		int n = rnd(4, 40), R = rnd(1, 5);
		vector<point3_t> p;
		bool full = false;
		while (!full) { // distinct points, not all coplanar
			set<tuple<int, int, int>> seen; p.clear();
			for (int i = 0; i < n; ++i) {
				int x = rnd(-R, R), y = rnd(-R, R), z = rnd(-R, R);
				if (seen.insert({x, y, z}).second) p.push_back(point3_t(x, y, z));
			}
			for (int i = 3; i < (int)p.size() && !full; ++i) for (int j = 2; j < i && !full; ++j)
				full = dblcmp(volume(p[0], p[1], p[j], p[i])) != 0;
			if (!full) for (int k = 2; k < (int)p.size() && !full; ++k) for (int j = 1; j < k && !full; ++j) for (int i = 0; i < j && !full; ++i)
				for (int l = 0; l < (int)p.size() && !full; ++l) full = dblcmp(volume(p[i], p[j], p[k], p[l])) != 0;
		}
		n = p.size(); pt = p; hull3d();
		auto fail = [&](const char *what) {
			printf("seed %u it %d: %s\n", seed, it, what);
			for (auto &q : p) printf("(%g,%g,%g) ", q.x, q.y, q.z);
			puts(""); return 1;
		};
		map<pair<int, int>, int> edges; double V = 0;
		for (auto &f : fc) {
			if (dblcmp(area(pt[f.a], pt[f.b], pt[f.c])) == 0) return fail("degenerate face");
			for (int i = 0; i < n; ++i) if (dblcmp(volume(pt[i], pt[f.a], pt[f.b], pt[f.c])) > 0) return fail("point outside face");
			++edges[{f.a, f.b}], ++edges[{f.b, f.c}], ++edges[{f.c, f.a}];
			V -= volume(pt[0], pt[f.a], pt[f.b], pt[f.c]);
		}
		for (auto [e, k] : edges) if (k != 1 || edges[{e.second, e.first}] != 1) return fail("surface not closed");
		if (it % 20 == 0) { // grid volume: inside iff on inner side of every supporting plane from O(n^3) triples
			set<array<ll, 4>> planes; // integer normal and offset, reduced by gcd
			for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) for (int k = 0; k < n; ++k) {
				point3_t nm = cross(p[j] - p[i], p[k] - p[i]);
				array<ll, 4> h = {llround(nm.x), llround(nm.y), llround(nm.z), 0};
				if (!h[0] && !h[1] && !h[2]) continue;
				h[3] = h[0] * llround(p[i].x) + h[1] * llround(p[i].y) + h[2] * llround(p[i].z);
				ll g = 0; for (ll v : h) g = __gcd(g, llabs(v));
				for (ll &v : h) v /= g;
				if (planes.count(h)) continue;
				bool ok = true; // all points on the side n . x <= d
				for (int l = 0; l < n && ok; ++l) ok = h[0] * llround(p[l].x) + h[1] * llround(p[l].y) + h[2] * llround(p[l].z) <= h[3];
				if (ok) planes.insert(h);
			}
			const int G = 30; double s = 2.0 * R / G, cnt = 0;
			for (int x = 0; x < G; ++x) for (int y = 0; y < G; ++y) for (int z = 0; z < G; ++z) {
				point3_t q(-R + (x + .5) * s, -R + (y + .5) * s, -R + (z + .5) * s); bool in = true;
				for (auto &h : planes) if (h[0] * q.x + h[1] * q.y + h[2] * q.z > h[3]) { in = false; break; }
				cnt += in;
			}
			if (fabs(cnt * s * s * s - V) > 0.1 * max(1.0, V) + 0.5) return fail("volume differs from grid");
		}
	}
	puts("OK convex-hull-3d");
}
