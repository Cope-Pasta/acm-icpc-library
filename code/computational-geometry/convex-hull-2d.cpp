bool cmp(const point_t p1, const point_t p2) {
	return dblcmp(p1.y - p2.y) == 0 ? p1.x < p2.x : p1.y < p2.y;
}

vector<point_t> graham(vector<point_t> p) { // CCW from lowest point, co-lined points ignored
	sort(p.begin(), p.end(), cmp);
	p.erase(unique(p.begin(), p.end(), [](point_t a, point_t b) { return dblcmp(a.x - b.x) == 0 && dblcmp(a.y - b.y) == 0; }), p.end());
	int n = p.size();
	if (n < 3) return p;
	vector<point_t> sk;
	for (int i = 0; i < n; ++i) {
		while (sk.size() >= 2 && dblcmp(cross(p[i] - sk[sk.size() - 2], sk.back() - sk[sk.size() - 2])) >= 0) sk.pop_back();
		sk.push_back(p[i]);
	}
	for (int i = n - 2, t = sk.size(); i >= 0; --i) {
		while ((int)sk.size() > t && dblcmp(cross(p[i] - sk[sk.size() - 2], sk.back() - sk[sk.size() - 2])) >= 0) sk.pop_back();
		sk.push_back(p[i]);
	}
	sk.pop_back();
	return sk;
}
