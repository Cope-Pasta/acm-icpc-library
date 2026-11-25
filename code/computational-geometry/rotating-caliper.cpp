// Calculate the maximum distance of a point set. p: CCW convex hull
double rotate_caliper(const vector<point_t> &p) {
	int n = p.size(), to = 0;
	double ans = 0;
	for (int i = 0; i < n; ++i) {
		point_t a = p[i], b = p[(i + 1) % n];
		while ((to + 1) % n != i) {
			if (dblcmp(cross(b - a, p[(to + 1) % n] - a) - cross(b - a, p[to] - a)) >= 0) to = (to + 1) % n;
			else break;
		}
		ans = max(ans, dist(a, p[to]));
		ans = max(ans, dist(b, p[to]));
	}
	return ans;
}
