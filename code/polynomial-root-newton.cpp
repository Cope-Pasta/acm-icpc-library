// Real roots of c[0] + c[1]x + ... + c[n]x^n, n >= 1. Like polynomial-root,
// but brackets are refined by Newton (bisection if a step leaves the bracket).
// Fewer evaluations. Needs eps, dblcmp.
double val(const vector<double> &c, double x) {
	double s = 0;
	for (int i = c.size(); i--;) s = s * x + c[i];
	return s;
}
vector<double> solve(vector<double> c) {
	while (c.back() == 0) c.pop_back();
	int n = c.size() - 1;
	double b = 2, t = c[n]; // b: bound on |root|
	for (double &x : c) x /= t;
	if (n == 1) return {-c[0]};
	vector<double> d(n), r;
	for (int i = 0; i < n; i++)
		d[i] = c[i + 1] * (i + 1), b = max(b, 2 * pow(fabs(c[i]), 1.0 / (n - i)));
	auto dr = solve(d);
	dr.insert(dr.begin(), -b), dr.push_back(b);
	for (int i = 0; i + 1 < dr.size(); i++) {
		double l = dr[i], h = dr[i + 1], x = (l + h) / 2;
		int sl = dblcmp(val(c, l)), sh = dblcmp(val(c, h));
		if (sl * sh > 0) continue;
		if (!sl || !sh) x = sl ? h : l;
		else
			for (int it = 0; it < 100; it++) {
				double f = val(c, x), y = x - f / val(d, x);
				int s = dblcmp(f);
				if (!s) break;
				(s == sl ? l : h) = x;
				if (!(l < y && y < h)) y = (l + h) / 2;
				swap(x, y);
				if (fabs(x - y) < eps) break;
			}
		r.push_back(x);
	}
	return r;
}
