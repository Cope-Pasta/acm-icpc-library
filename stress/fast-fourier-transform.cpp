#include "check/prelude.h"
#include "code/fast-fourier-transform.cpp"

int main() {
	unsigned seed = 6; mt19937 rng(seed); preprocess();
	for (int it = 0; it < 3000; ++it) {
		int n = rng() % 300 + 1, m = rng() % 300 + 1, lim = it % 3 ? 1000 : 100000;
		if (it % 10 == 0) n = 1;
		vector<int> a(n), b(m);
		for (auto &x : a) x = rng() % lim;
		for (auto &x : b) x = rng() % lim;
		vector<ll> want(n + m - 1);
		for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) want[i + j] += (ll)a[i] * b[j];
		if (multiply(a, b) != want) { printf("seed %u it %d n %d m %d\n", seed, it, n, m); return 1; }
	}
	vector<int> a(100000, 100000), b(100000, 100000); // large: 1e15 coefficients
	auto r = multiply(a, b);
	if (r[99999] != 100000LL * 100000 * 100000) { printf("large %lld\n", r[99999]); return 1; }
	puts("OK fast-fourier-transform");
}
