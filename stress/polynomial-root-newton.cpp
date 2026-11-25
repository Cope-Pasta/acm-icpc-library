#include "check/prelude.h"
const double eps = 1e-9;
int dblcmp(double x) { return x < -eps ? -1 : x > eps; }
#include "code/polynomial-root-newton.cpp"

int main() { // polynomials with known distinct real roots (spaced >= 0.5), times a scale
	unsigned seed = 4; mt19937 rng(seed);
	for (int it = 0; it < 3000; ++it) {
		int n = rng() % 5 + 1; vector<double> rt;
		for (int i = 0; i < n; ++i) rt.push_back((int)(rng() % 21) - 10 + (rng() % 2 ? 0.5 : 0));
		sort(rt.begin(), rt.end()); rt.erase(unique(rt.begin(), rt.end()), rt.end());
		vector<double> c = {1};
		for (double r : rt) { vector<double> d(c.size() + 1); for (size_t i = 0; i < c.size(); ++i) d[i + 1] += c[i], d[i] -= r * c[i]; c = d; }
		if (it % 3 == 0) { vector<double> d(c.size() + 2); for (size_t i = 0; i < c.size(); ++i) d[i] += c[i], d[i + 2] += c[i]; c = d; } // times (x^2 + 1): no new real roots
		double sc = (int)(rng() % 9) - 4; if (sc == 0) sc = 3;
		for (auto &x : c) x *= sc;
		auto got = solve(c); sort(got.begin(), got.end());
		got.erase(unique(got.begin(), got.end(), [](double a, double b) { return fabs(a - b) < 1e-6; }), got.end());
		bool ok = got.size() == rt.size();
		for (size_t i = 0; ok && i < rt.size(); ++i) ok = fabs(got[i] - rt[i]) < 1e-5;
		if (!ok) {
			printf("seed %u it %d roots:", seed, it); for (double r : rt) printf(" %g", r);
			printf(" got:"); for (double r : got) printf(" %g", r); puts(""); return 1;
		}
	}
	puts("OK polynomial-root-newton");
}
