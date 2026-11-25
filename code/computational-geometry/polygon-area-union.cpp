// area of the union of simple polygons (any orientation, not necessarily convex), O(N^2 log N) for N vertices
double polyUnion(vector<vector<point_t>> p) {
	double r = 0;
	for (auto &q : p) {
		double s = 0;
		for (int i = 0, n = q.size(); i < n; ++i) s += cross(q[i], q[(i + 1) % n]);
		if (s < 0) reverse(all(q));
	}
	for (int i = 0; i < (int)p.size(); ++i)
		for (int v = 0, n = p[i].size(); v < n; ++v) {
			point_t a = p[i][v], b = p[i][(v + 1) % n];
			vector<pair<double, int>> e = {{0, 0}, {1, 0}}; // parts of edge ab covered by other polygons
			for (int j = 0; j < (int)p.size(); ++j)
				for (int u = 0, m = p[j].size(); u < m && i != j; ++u) {
					point_t c = p[j][u], d = p[j][(u + 1) % m];
					int sc = dblcmp(cross(b - a, c - a)), sd = dblcmp(cross(b - a, d - a));
					if (sc != sd) {
						double sa = cross(d - c, a - c), sb = cross(d - c, b - c);
						if (min(sc, sd) < 0) e.push_back({sa / (sa - sb), sc > sd ? 1 : -1});
					} else if (!sc && j < i && dot(b - a, d - c) > 0) { // shared edge: count once
						double l = dot(b - a, b - a);
						e.push_back({dot(c - a, b - a) / l, 1}), e.push_back({dot(d - a, b - a) / l, -1});
					}
				}
			sort(all(e));
			double s = 0;
			for (int k = 1, cnt = e[0].second; k < (int)e.size(); cnt += e[k++].second)
				if (!cnt) s += clamp(e[k].first, 0.0, 1.0) - clamp(e[k - 1].first, 0.0, 1.0);
			r += cross(a, b) * s;
		}
	return r / 2;
}
