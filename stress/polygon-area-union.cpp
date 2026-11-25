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

double slabs(const vector<vector<point_t>> &ps) { // exact for any simple polygons: union length is linear in x between event xs
	vector<double> xs;
	vector<pair<point_t, point_t>> es;
	for (auto &p : ps) for (int i = 0; i < (int)p.size(); ++i) es.push_back({p[i], p[(i + 1) % p.size()]}), xs.push_back(p[i].x);
	for (auto &[a, b] : es) for (auto &[c, d] : es) {
		double den = cross(b - a, d - c);
		if (fabs(den) < 1e-12) continue;
		double t = cross(c - a, d - c) / den, u = cross(c - a, b - a) / den;
		if (t > -1e-9 && t < 1 + 1e-9 && u > -1e-9 && u < 1 + 1e-9) xs.push_back(a.x + (b.x - a.x) * t);
	}
	sort(xs.begin(), xs.end());
	double res = 0;
	for (int k = 0; k + 1 < (int)xs.size(); ++k) {
		double x = (xs[k] + xs[k + 1]) / 2, w = xs[k + 1] - xs[k];
		if (w < 1e-12) continue;
		vector<pair<double, double>> iv;
		for (auto &p : ps) { // even-odd crossings of the vertical line x
			vector<double> ys;
			for (int i = 0; i < (int)p.size(); ++i) {
				point_t a = p[i], b = p[(i + 1) % p.size()];
				if ((a.x < x) != (b.x < x)) ys.push_back(a.y + (b.y - a.y) * (x - a.x) / (b.x - a.x));
			}
			sort(ys.begin(), ys.end());
			for (int i = 0; i + 1 < (int)ys.size(); i += 2) iv.push_back({ys[i], ys[i + 1]});
		}
		sort(iv.begin(), iv.end());
		double len = 0, hi = -1e18;
		for (auto [l, r] : iv) { if (r > hi) len += r - max(l, hi), hi = r; }
		res += len * w;
	}
	return res;
}

vector<point_t> star(mt19937 &rng, int R) { // random simple polygon, star-shaped around (0.13, 0.07), either orientation
	int n = rng() % 8 + 3;
	vector<pair<double, point_t>> v;
	for (bool ok = false; !ok;) { // the center must be strictly inside: every angular gap < pi
		v.clear();
		for (int i = 0; i < n; ++i) {
			double a = (rng() % 100000) / 100000.0 * 2 * pi, r = (rng() % 1000 + 1) / 1000.0 * R;
			v.push_back({a, point_t(0.13 + r * cos(a), 0.07 + r * sin(a))});
		}
		sort(v.begin(), v.end(), [](auto &x, auto &y) { return x.first < y.first; });
		ok = v[0].first + 2 * pi - v.back().first < pi - 0.1;
		for (int i = 1; i < n; ++i) ok &= v[i].first - v[i - 1].first < pi - 0.1;
	}
	vector<point_t> p;
	for (auto &q : v) p.push_back(q.second);
	if (rng() % 2) reverse(p.begin(), p.end());
	return p;
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
			auto h = convexHull(pts);
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
	for (int it = 0; it < 500; ++it) { // non-convex polygons against the slab brute force
		int n = rnd(1, 4), R = rnd(1, 10);
		vector<vector<point_t>> ps;
		for (int i = 0; i < n; ++i) ps.push_back(star(rng, R));
		if (n > 1 && rnd(0, 3) == 0) ps[1] = ps[0]; // identical polygons
		double got = polyUnion(ps), exp = slabs(ps);
		if (fabs(got - exp) > 1e-6 * max(1.0, exp)) {
			printf("seed %u star it %d: got %.9f expected %.9f\n", seed, it, got, exp);
			for (auto &p : ps) { for (auto &q : p) printf("(%g,%g) ", q.x, q.y); puts(""); }
			return 1;
		}
	}
	puts("OK polygon-area-union");
}
