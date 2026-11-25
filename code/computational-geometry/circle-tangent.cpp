vector<point_t> tanCP(point_t c, double r, point_t p) { // tangent points from p; {p} if p is on the circle
	point_t v = p - c;
	double x = dot(v, v), d = x - r * r;
	if (d < -eps) return {};
	if (d < eps) return {p};
	point_t q = c + v * (r * r / x), w = v.rot90() * (r * sqrt(d) / x);
	return {q + w, q - w};
}
// common tangents as (point on C1, point on C2), outer ones first; touching circles give (p, p + dir)
// at the touching point; {} if concentric
vector<pair<point_t, point_t>> tanCC(point_t c1, double r1, point_t c2, double r2) {
	vector<pair<point_t, point_t>> res;
	if (!dblcmp(dist(c1, c2))) return res;
	auto add = [&](point_t p) {
		vector<point_t> ps = tanCP(c1, r1, p), qs = tanCP(c2, r2, p);
		for (int i = 0; i < (int)min(ps.size(), qs.size()); ++i) res.push_back({ps[i], dblcmp(dist(ps[i], qs[i])) ? qs[i] : ps[i] + (c2 - c1).rot90()});
	};
	if (dblcmp(r1 - r2)) add((c2 * r1 - c1 * r2) / (r1 - r2));
	else {
		point_t d = (c2 - c1).rot90() * (r1 / dist(c1, c2));
		res.push_back({c1 + d, c2 + d}), res.push_back({c1 - d, c2 - d});
	}
	add((c1 * r2 + c2 * r1) / (r1 + r2));
	return res;
}
