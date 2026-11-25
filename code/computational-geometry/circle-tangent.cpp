vector<point_t> tanCP(point_t c, double r, point_t p) {
	double x = dot(p - c, p - c);
	double d = x - r * r;
	vector<point_t> res;
	if (d < -eps) return res;
	if (d < eps) return {p}; // p on the circle
	point_t q1 = (p - c) * (r * r / x);
	point_t q2 = ((p - c) * (-r * sqrt(d) / x)).rot90(); // rot90: (-y, x)
	res.push_back(c + q1 - q2);
	res.push_back(c + q1 + q2);
	return res;
}

vector<seg_t> tanCC(point_t c1, double r1, point_t c2, double r2) { // touching circles: (p, p + dir) at the touching point
	vector<seg_t> res;
	if (dblcmp(dist(c1, c2)) == 0) return res; // concentric
	auto add = [&](point_t p) {
		vector<point_t> ps = tanCP(c1, r1, p), qs = tanCP(c2, r2, p);
		for (int i = 0; i < (int)ps.size() && i < (int)qs.size(); ++i)
			res.push_back(seg_t(ps[i], dblcmp(dist(ps[i], qs[i])) == 0 ? ps[i] + (c2 - c1).rot90() : qs[i]));
	};
	if (abs(r1 - r2) < eps) {
		point_t dir = c2 - c1;
		dir = (dir * (r1 / dir.l())).rot90();
		res.push_back(seg_t(c1 + dir, c2 + dir));
		res.push_back(seg_t(c1 - dir, c2 - dir));
	} else add(((c1 * -r2) + (c2 * r1)) / (r1 - r2));
	add(((c1 * r2) + (c2 * r1)) / (r1 + r2));
	return res;
}
