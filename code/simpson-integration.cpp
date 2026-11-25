// Adaptive Simpson: integral of f over [l, r], splitting until the local error estimate < 15 eps.
const double eps = 1e-15;
double f(double x) { return 0.0; }
double sim(double l, double r, double fl, double fm, double fr) { return (r - l) * (fl + 4 * fm + fr) / 6; }
double rec(double l, double r, double fl, double fm, double fr) {
	double m = (l + r) / 2, a = f((l + m) / 2), b = f((m + r) / 2), s = sim(l, r, fl, fm, fr), L = sim(l, m, fl, a, fm), R = sim(m, r, fm, b, fr);
	if (fabs(L + R - s) < 15 * eps) return L + R + (L + R - s) / 15;
	return rec(l, m, fl, a, fm) + rec(m, r, fm, b, fr);
}
double simpson(double l, double r) { return rec(l, r, f(l), f((l + r) / 2), f(r)); }
