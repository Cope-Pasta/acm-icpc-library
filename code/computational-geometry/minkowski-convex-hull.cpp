// Minkowski sum of hulls as convexHull returns them (CCW from the lowest, then leftmost point; size >= 1), O(n + m)
vector<point_t> minkowski(vector<point_t> a, vector<point_t> b) {
	int n = a.size(), m = b.size(), i = 0, j = 0;
	a.push_back(a[0]), a.push_back(a[1 % n]), b.push_back(b[0]), b.push_back(b[1 % m]);
	vector<point_t> h;
	while (i < n || j < m) {
		h.push_back(a[i] + b[j]);
		int c = dblcmp(cross(a[i + 1] - a[i], b[j + 1] - b[j]));
		if (c >= 0 && i < n) ++i;
		if (c <= 0 && j < m) ++j;
	}
	return h;
}
