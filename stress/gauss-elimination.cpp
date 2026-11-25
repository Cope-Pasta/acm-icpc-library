#include "check/prelude.h"
#include "code/gauss-elimination.cpp"

int main() {
	unsigned seed = 3; mt19937 rng(seed); uniform_real_distribution<double> U(-10, 10);
	for (int it = 0; it < 5000; ++it) {
		int n = rng() % 7 + 1;
		vector<vector<double>> a(n, vector<double>(n)); vector<double> x(n), b(n, 0);
		for (int i = 0; i < n; ++i) { for (auto &v : a[i]) v = U(rng); a[i][i] += (rng() % 2 ? 1 : -1) * 40; } // diagonally dominant: nonsingular
		if (it % 4 == 0) for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if (i != j && rng() % 2) a[i][j] = 0; // sparse, needs pivoting less
		for (auto &v : x) v = U(rng);
		for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) b[i] += a[i][j] * x[j];
		vector<vector<double>> g = a;
		for (int i = 0; i < n; ++i) g[i].push_back(b[i]);
		shuffle(g.begin(), g.end(), rng); // row order must not matter
		gauss(g);
		for (int i = 0; i < n; ++i) if (fabs(g[i][n] - x[i]) > 1e-7) { printf("seed %u it %d n %d: x[%d] want %.9f got %.9f\n", seed, it, n, i, x[i], g[i][n]); return 1; }
	}
	vector<vector<double>> g = {{0, 1, 2}, {1, 0, 3}}; gauss(g); // zero on the diagonal
	if (fabs(g[0][2] - 3) > 1e-9 || fabs(g[1][2] - 2) > 1e-9) { puts("pivot"); return 1; }
	puts("OK gauss-elimination");
}
