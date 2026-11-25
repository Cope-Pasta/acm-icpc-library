double dot(point3_t a, point3_t b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
point3_t cross(point3_t a, point3_t b) { return point3_t(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
double volume(point3_t a, point3_t b, point3_t c, point3_t d) { return dot(cross(b - a, c - a), d - a) / 6; } // signed
double area(point3_t a, point3_t b, point3_t c) { return cross(b - a, c - a).length() / 2; }
// line where planes (point p1, normal o1) and (p2, o2) meet, as two points; throws if parallel
pair<point3_t, point3_t> isFF(point3_t p1, point3_t o1, point3_t p2, point3_t o2) {
	point3_t e = cross(o1, o2), v = cross(o1, e);
	double d = dot(o2, v);
	if (fabs(d) < eps) throw -1;
	point3_t q = p1 + v * (dot(o2, p2 - p1) / d);
	return {q, q + e};
}
double distLL(point3_t p, point3_t u, point3_t q, point3_t v) { // lines p + s*u, q + t*v
	point3_t n = cross(u, v);
	if (!dblcmp(n.length())) return cross(q - p, u).length() / u.length();
	return fabs(dot(q - p, n)) / n.length();
}
