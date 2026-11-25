// Assume |r1 - r2| <= d <= r1 + r2, d > 0; first is right of c1 -> c2, second is left
pair<point_t, point_t> isCC(point_t c1, point_t c2, double r1, double r2) {
	point_t v = c2 - c1;
	double d2 = dot(v, v), a = (d2 + r1 * r1 - r2 * r2) / (2 * d2);
	double h = sqrt(max(0.0, r1 * r1 / d2 - a * a));
	point_t m = c1 + v * a, w = v.rot90() * h;
	return make_pair(m - w, m + w);
}

int testCC(point_t c1, point_t c2, double r1, double r2) {
	double d = dist(c1, c2);
	if (dblcmp(r1 + r2 - d) <= 0) return 1; // not intersected or tged
	if (dblcmp(r1 + d - r2) <= 0) return 2; // C1 inside C2
	if (dblcmp(r2 + d - r1) <= 0) return 3; // C2 inside C1
	return 0; // intersected
}

vector<point_t> isCL(point_t a, point_t b, point_t o, double r) { // first hit of ray a->b, empty if none
	double x0 = o.x, y0 = o.y;
	double x1 = a.x, y1 = a.y;
	double x2 = b.x, y2 = b.y;
	double dx = x2 - x1, dy = y2 - y1;
	double A = dx * dx + dy * dy;
	double B = 2 * dx * (x1 - x0) + 2 * dy * (y1 - y0);
	double C = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) - r * r;
	double delta = B * B - 4 * A * C;
	if (delta >= 0) {
		delta = sqrt(delta);
		double t1 = (-B - delta) / 2 / A;
		double t2 = (-B + delta) / 2 / A;
		if (dblcmp(t1) >= 0) return {point_t(x1 + t1 * dx, y1 + t1 * dy)}; // Ray
		if (dblcmp(t2) >= 0) return {point_t(x1 + t2 * dx, y1 + t2 * dy)};
	}
	return {};
}