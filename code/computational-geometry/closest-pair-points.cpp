// closest pair distance by sweep line, O(n log n), n >= 2
double closestPair(vector<point_t> p) {
	sort(all(p), [](point_t a, point_t b) { return a.x < b.x; });
	set<pair<double, double>> s; // (y, x) of points with x in [q.x - d, q.x]
	double d = maxdbl;
	for (int i = 0, j = 0; i < (int)p.size(); s.insert({p[i].y, p[i].x}), ++i) {
		for (; p[i].x - p[j].x > d; ++j) s.erase({p[j].y, p[j].x});
		for (auto it = s.lower_bound({p[i].y - d, -maxdbl});
		    it != s.end() && it->first <= p[i].y + d; ++it)
			d = min(d, dist(p[i], point_t(it->second, it->first)));
	}
	return d;
}
