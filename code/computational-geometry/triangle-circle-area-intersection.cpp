// signed area of circle(c, r) ∩ triangle(c, a, b); summing over the edges of a polygon gives polygon ∩ circle
double areaTC(point_t c, double r, point_t a, point_t b) {
	a = a - c, b = b - c;
	point_t d = b - a;
	double A = dot(d, d), B = dot(a, d), C = dot(a, a) - r * r;
	if (A == 0) return 0;
	double s = sqrt(max(0.0, B * B - A * C)); // a + d*t on the circle at t = (-B -+ s) / A
	point_t p = a + d * min(1.0, max(0.0, (-B - s) / A)), q = a + d * min(1.0, max(0.0, (-B + s) / A));
	auto sector = [&](point_t u, point_t v) { return r * r * atan2(cross(u, v), dot(u, v)) / 2; };
	return sector(a, p) + cross(p, q) / 2 + sector(q, b); // [p, q] is the part of ab inside the circle
}

double areaTC(point_t c, double r, point_t p1, point_t p2, point_t p3) {
	return areaTC(c, r, p1, p2) + areaTC(c, r, p2, p3) + areaTC(c, r, p3, p1);
}
