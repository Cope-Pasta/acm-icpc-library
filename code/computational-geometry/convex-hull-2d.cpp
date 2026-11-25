// monotone chain, O(n log n): CCW from the lowest (then leftmost) point, duplicate and collinear points dropped
vector<point_t> convexHull(vector<point_t> p) {
	sort(all(p), [](point_t a, point_t b) { return a.y < b.y || (a.y == b.y && a.x < b.x); });
	if (p.size() < 2) return p;
	vector<point_t> h(p.size() + 1);
	int s = 0, t = 0;
	for (int k = 2; k--; s = --t, reverse(all(p)))
		for (point_t q : p) {
			while (t >= s + 2 && dblcmp(cross(h[t - 1] - h[t - 2], q - h[t - 2])) <= 0) --t;
			h[t++] = q;
		}
	return {h.begin(), h.begin() + t - (t == 2 && !dblcmp(dist(h[0], h[1])))};
}
