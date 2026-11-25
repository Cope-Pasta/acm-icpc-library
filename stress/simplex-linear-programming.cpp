#include "check/prelude.h"
const double eps = 1e-9;
#include "code/simplex-linear-programming.cpp"

// brute: max over vertices of {Ax <= b, x >= 0}; returns -1 if infeasible (box rows keep it bounded)
double brute(const vector<vector<double>> &a, const vector<double> &b, const vector<double> &c) {
	int n = a.size(), m = c.size(); vector<vector<double>> A = a; vector<double> B = b;
	for (int j = 0; j < m; ++j) { vector<double> r(m); r[j] = -1; A.push_back(r); B.push_back(0); }
	int t = A.size(); double best = -1e18; bool any = false;
	for (int mask = 0; mask < 1 << t; ++mask) if (__builtin_popcount(mask) == m) {
		vector<vector<double>> g; for (int i = 0; i < t; ++i) if (mask >> i & 1) { g.push_back(A[i]); g.back().push_back(B[i]); }
		bool sing = false;
		for (int i = 0; i < m && !sing; ++i) {
			int p = i; for (int j = i; j < m; ++j) if (fabs(g[j][i]) > fabs(g[p][i])) p = j;
			if (fabs(g[p][i]) < 1e-9) { sing = true; break; }
			swap(g[i], g[p]);
			for (int j = 0; j < m; ++j) if (j != i) { double f = g[j][i] / g[i][i]; for (int k = i; k <= m; ++k) g[j][k] -= f * g[i][k]; }
		}
		if (sing) continue;
		vector<double> x(m); for (int i = 0; i < m; ++i) x[i] = g[i][m] / g[i][i];
		bool ok = true; for (int i = 0; i < t && ok; ++i) { double s = 0; for (int j = 0; j < m; ++j) s += A[i][j] * x[j]; ok = s <= B[i] + 1e-7; }
		if (!ok) continue;
		double v = 0; for (int j = 0; j < m; ++j) v += c[j] * x[j];
		best = max(best, v), any = true;
	}
	return any ? best : -1;
}

int main() {
	unsigned seed = 8; mt19937 rng(seed);
	int infeasible = 0;
	for (int it = 0; it < 4000; ++it) {
		int m = rng() % 3 + 1, n = rng() % 4;
		vector<vector<double>> a; vector<double> b, c(m);
		for (int i = 0; i < n; ++i) { vector<double> r(m); for (auto &v : r) v = (int)(rng() % 11) - 5; a.push_back(r); b.push_back((int)(rng() % 21) - 4); }
		for (int j = 0; j < m; ++j) { vector<double> r(m); r[j] = 1; a.push_back(r); b.push_back(rng() % 10 + 1); } // box: bounded
		for (auto &v : c) v = (int)(rng() % 11) - 5;
		double want = brute(a, b, c), got = simplex(a, b, c);
		if (want == -1) ++infeasible;
		if (fabs(want - got) > 1e-6) { printf("seed %u it %d n %d m %d: want %.6f got %.6f\n", seed, it, (int)a.size(), m, want, got); return 1; }
	}
	if (simplex({{1, -1}}, {1}, {1, 1}) != -1) { puts("unbounded"); return 1; } // x - y <= 1, max x + y
	if (simplex({{1}, {-1}}, {1, -2}, {1}) != -1) { puts("infeasible"); return 1; } // x <= 1, x >= 2
	if (infeasible == 0) { puts("no infeasible cases generated"); return 1; }
	puts("OK simplex-linear-programming");
}
