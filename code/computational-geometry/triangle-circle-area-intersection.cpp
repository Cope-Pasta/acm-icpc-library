// signed area of circle(c, r) & triangle(c, a, b); sum over the edges of a polygon for polygon & circle
double areaTC(point_t c, double r, point_t a, point_t b) {
	a = a - c, b = b - c;
	point_t d = b - a;
	double A = dot(d, d), B = dot(a, d), s = sqrt(max(0.0, B * B - A * (dot(a, a) - r * r)));
	if (A == 0) return 0;
	point_t p = a + d * clamp((-B - s) / A, 0.0, 1.0), q = a + d * clamp((-B + s) / A, 0.0, 1.0); // [p, q]: ab inside circle
	auto sector = [&](point_t u, point_t v) { return r * r * atan2(cross(u, v), dot(u, v)) / 2; };
	return sector(a, p) + cross(p, q) / 2 + sector(q, b);
}
