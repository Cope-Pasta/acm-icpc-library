// Cut CCW convex polygon p by the half-plane left of a->b, O(n).
// O(n^2) half-plane intersection: start from a big CCW box
// {(-M,-M), (M,-M), (M,M), (-M,M)} and cut by each half-plane.
vector<point_t> cut(const vector<point_t> &p, point_t a, point_t b) {
	vector<point_t> r;
	for (int i = 0, n = p.size(); i < n; ++i) {
		point_t c = p[i], d = p[(i + 1) % n];
		int x = dblcmp(cross(b - a, c - a)), y = dblcmp(cross(b - a, d - a));
		if (x >= 0) r.push_back(c);
		if (x * y < 0) r.push_back(isLL(a, b, c, d));
	}
	return r;
}
