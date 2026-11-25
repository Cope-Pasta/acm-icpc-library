double dac(vector<point_t> &p, int l, int r) { // [l, r]
	double d = 10e100;
	if (r - l <= 3) {
		for (int i = l; i <= r; ++i) {
			for (int j = i + 1; j <= r; ++j) {
				d = min(d, dist2(p[i], p[j]));
			}
		}
		sort(p.begin() + l, p.begin() + r + 1, cmpY);
	} else {
		int mid = (l + r) / 2;
		double mx = p[mid].x;
		d = min(dac(p, l, mid), dac(p, mid + 1, r));
		inplace_merge(p.begin() + l, p.begin() + mid + 1, p.begin() + r + 1, cmpY);
		vector<point_t> tmp;
		for (int i = l; i <= r; ++i) {
			if ((p[i].x - mx) * (p[i].x - mx) <= d) tmp.push_back(p[i]);
		}
		int cnt = tmp.size();
		for (int i = 0; i < cnt; ++i) {
			for (int j = 1; j <= 8 && j + i < cnt; ++j) {
				d = min(d, dist2(tmp[i], tmp[j + i]));
			}
		}
	}
	return d;
}

double cal(vector<point_t> p) {
	sort(p.begin(), p.end(), cmpX);
	return sqrt(dac(p, 0, p.size() - 1));
}