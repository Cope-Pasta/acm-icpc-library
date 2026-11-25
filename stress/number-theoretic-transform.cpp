#include "check/prelude.h"
int inverse(int a, int m) { LL r = 1, x = a; for (int e = m - 2; e; e >>= 1, x = x * x % m) if (e & 1) r = r * x % m; return r; }
#include "code/number-theoretic-transform.cpp"

int main() {
	unsigned seed = 7; mt19937 rng(seed);
	for (int it = 0; it < 3000; ++it) {
		int n = rng() % 200 + 1, m = rng() % 200 + 1, sz = 1;
		while (sz < n + m - 1) sz <<= 1;
		vector<int> a(sz), b(sz);
		for (int i = 0; i < n; ++i) a[i] = rng() % mod;
		for (int i = 0; i < m; ++i) b[i] = rng() % mod;
		vector<LL> want(n + m - 1);
		for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) want[i + j] = (want[i + j] + (LL)a[i] * b[j]) % mod;
		vector<int> c = a; fft(c, false); fft(c, true);
		if (c != a) { printf("seed %u it %d: inverse(fft) != id\n", seed, it); return 1; }
		fft(a, false), fft(b, false);
		for (int i = 0; i < sz; ++i) a[i] = (LL)a[i] * b[i] % mod;
		fft(a, true);
		for (int i = 0; i < n + m - 1; ++i) if (a[i] != want[i]) { printf("seed %u it %d n %d m %d at %d\n", seed, it, n, m, i); return 1; }
	}
	vector<int> big(1 << 20, 1); fft(big, false); fft(big, true);
	if (count(big.begin(), big.end(), 1) != 1 << 20) { puts("size 2^20"); return 1; }
	puts("OK number-theoretic-transform");
}
