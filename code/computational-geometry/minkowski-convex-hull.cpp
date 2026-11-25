vector<point_t> minkowski(const vector<point_t> &h1, vector<point_t> h2) { // h1, h2 CCW hulls
	int n = h1.size(), m = h2.size();
	point_t c = accumulate(all(h2), point_t(0, 0)) / m;
	for (auto &q : h2) q = q - c;
	int cur = -1;
	for (int i = 0; i < m; ++i) {
		if (dblcmp(cross(h2[i], h1[0] - h1[n - 1])) >= 0) {
			if (cur == -1 || cross(h2[i], h1[0] - h1[n - 1]) > cross(h2[cur], h1[0] - h1[n - 1])) cur = i;
		}
	}
	vector<point_t> h;
	for (int i = 0; i < n; ++i) {
		point_t e = h1[(i + 1) % n] - h1[i];
		while (true) {
			h.push_back(h1[i] + h2[cur] + c);
			int next = (cur + 1) % m;
			if (dblcmp(cross(h2[cur], e)) < 0) cur = next;
			else {
				if (cross(h2[next], e) > cross(h2[cur], e)) cur = next;
				else break;
			}
		}
	}
	return graham(h);
}
