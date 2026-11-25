// implementation of (dblcmp,dist,cross,dot,rot90) is trivial; CCW is positive
double angle(point_t a, point_t b) { return atan2(cross(a, b), dot(a, b)); } // signed angle a->b in (-pi,pi]
point_t rotate(point_t p, double a) { return p * cos(a) + p.rot90() * sin(a); }
bool onSeg(point_t p, point_t a, point_t b) { return !dblcmp(cross(a - p, b - p)) && dblcmp(dot(a - p, b - p)) <= 0; }
// 1 proper intersection, -1 touching (endpoint/collinear overlap), 0 none
int testSS(point_t a, point_t b, point_t c, point_t d) {
	if (dblcmp(cross(b - a, c - a)) * dblcmp(cross(b - a, d - a)) < 0 && dblcmp(cross(d - c, a - c)) * dblcmp(cross(d - c, b - c)) < 0) return 1;
	return onSeg(c, a, b) || onSeg(d, a, b) || onSeg(a, c, d) || onSeg(b, c, d) ? -1 : 0;
}
// lines ab, cd; must not be parallel (check dblcmp(cross(b - a, d - c)))
point_t isLL(point_t a, point_t b, point_t c, point_t d) {
	point_t u = b - a, v = d - c;
	return a + u * (cross(c - a, v) / cross(u, v));
}
point_t circumcenter(point_t a, point_t b, point_t c) {
	b = b - a, c = c - a;
	double d = 2 * cross(b, c);
	assert(dblcmp(d)); // collinear
	return a + (b * dot(c, c) - c * dot(b, b)).rot90() / d;
}
point_t incenter(point_t a, point_t b, point_t c) {
	double x = dist(b, c), y = dist(c, a), z = dist(a, b);
	assert(dblcmp(cross(b - a, c - a))); // collinear
	return (a * x + b * y + c * z) / (x + y + z);
}
point_t orthocenter(point_t a, point_t b, point_t c) { return a + b + c - circumcenter(a, b, c) * 2; }
bool insidePoly(const vector<point_t> &p, point_t t) { // any simple polygon, boundary is inside
	bool in = 0;
	for (int i = 0, n = p.size(); i < n; ++i) {
		point_t a = p[i], b = p[(i + 1) % n];
		if (onSeg(t, a, b)) return 1;
		in ^= ((a.y < t.y) - (b.y < t.y)) * cross(a - t, b - t) > 0;
	}
	return in;
}
bool insideConvex(const vector<point_t> &p, point_t t) { // O(log n), CCW, no 3 collinear, boundary is inside
	int n = p.size(), l = 1, r = n - 1;
	if (n < 3) return onSeg(t, p[0], p[r]);
	if (dblcmp(cross(p[1] - p[0], t - p[0])) < 0 || dblcmp(cross(p[r] - p[0], t - p[0])) > 0) return 0;
	while (r - l > 1) {
		int m = (l + r) / 2;
		(dblcmp(cross(p[m] - p[0], t - p[0])) >= 0 ? l : r) = m;
	}
	return dblcmp(cross(p[r] - p[l], t - p[l])) >= 0;
}
