// Assume |r1 - r2| <= d <= r1 + r2, d > 0; first is right of c1 -> c2, second is left
pair<point_t, point_t> isCC(point_t c1, point_t c2, double r1, double r2) {
	point_t v = c2 - c1;
	double d2 = dot(v, v), a = (d2 + r1 * r1 - r2 * r2) / (2 * d2), h = sqrt(max(0.0, r1 * r1 / d2 - a * a));
	point_t m = c1 + v * a, w = v.rot90() * h;
	return {m - w, m + w};
}
// 0 intersecting, 1 apart or externally touching, 2 C1 inside C2, 3 C2 inside C1 (touching counts)
int testCC(point_t c1, point_t c2, double r1, double r2) {
	double d = dist(c1, c2);
	return dblcmp(r1 + r2 - d) <= 0 ? 1 : dblcmp(r1 + d - r2) <= 0 ? 2
	    : dblcmp(r2 + d - r1) <= 0                                 ? 3
	                                                               : 0;
}
vector<point_t> isCL(point_t a, point_t b, point_t o, double r) { // first hit of ray a->b, {} if none
	point_t d = b - a, w = a - o;
	double A = dot(d, d), B = dot(w, d), D = B * B - A * (dot(w, w) - r * r);
	if (D >= 0)
		for (double t : {(-B - sqrt(D)) / A, (-B + sqrt(D)) / A})
			if (dblcmp(t) >= 0) return {a + d * t};
	return {};
}
