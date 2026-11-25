// O(n^2 log n), no two identical circles. testCC: circle-intersection.
// Returns a: a[k] = area covered by exactly k circles.
vector<double> circleUnion(const vector<point_t> &c, const vector<double> &r) {
	int n = c.size();
	vector<double> a(n + 1);
	for (int i = 0; i < n; ++i) {
		int cur = 0; // circles covering the current arc of circle i, besides i
		vector<pair<double, int>> e = {{-pi, 0}, {pi, 0}}; // (angle, -delta)
		for (int j = 0; j < n; ++j) {
			int t = j == i ? 1 : testCC(c[i], c[j], r[i], r[j]);
			if (t == 2) ++cur;
			if (t) continue;
			double d = dist(c[i], c[j]),
			       m = atan2(c[j].y - c[i].y, c[j].x - c[i].x),
			       h = acos((r[i] * r[i] + d * d - r[j] * r[j]) / (2 * r[i] * d));
			double s = m - h < -pi ? m - h + 2 * pi : m - h, f = s + 2 * h;
			if (f > pi) ++cur, f -= 2 * pi; // arc wraps around -pi
			e.push_back({s, -1}), e.push_back({f, 1});
		}
		sort(all(e));
		auto P = [&](double t) { return c[i] + point_t(cos(t), sin(t)) * r[i]; };
		for (int k = 0; k + 1 < (int)e.size(); ++k) {
			cur -= e[k].second;
			double x = e[k].first, y = e[k + 1].first,
			       w = cross(P(x), P(y)) / 2 +
			    r[i] * r[i] * (y - x - sin(y - x)) / 2;
			a[cur] -= w, a[cur + 1] += w;
		}
	}
	return a;
}
