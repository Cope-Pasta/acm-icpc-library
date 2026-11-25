// minimum enclosing circle (center, radius), expected O(n), n >= 1. circumcenter: common-2d
pair<point_t, double> minCircle(vector<point_t> p) {
	shuffle(all(p), mt19937(time(0)));
	point_t o = p[0];
	double r = 0;
	auto out = [&](point_t q) { return dblcmp(dist(o, q) - r) > 0; };
	for (int i = 0; i < (int)p.size(); ++i)
		if (out(p[i])) {
			o = p[i], r = 0;
			for (int j = 0; j < i; ++j)
				if (out(p[j])) {
					o = (p[i] + p[j]) / 2, r = dist(o, p[i]);
					for (int k = 0; k < j; ++k)
						if (out(p[k])) o = circumcenter(p[i], p[j], p[k]), r = dist(o, p[i]);
				}
		}
	return {o, r};
}
