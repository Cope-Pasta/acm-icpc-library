// diameter (max point distance) of a CCW convex hull, O(n)
double rotate_caliper(const vector<point_t> &p) {
	int n = p.size(), j = 0;
	double r = 0;
	for (int i = 0; i < n; ++i) {
		point_t a = p[i], b = p[(i + 1) % n];
		while ((j + 1) % n != i && dblcmp(cross(b - a, p[(j + 1) % n] - p[j])) >= 0)
			j = (j + 1) % n;
		r = max({r, dist(a, p[j]), dist(b, p[j])});
	}
	return r;
}
