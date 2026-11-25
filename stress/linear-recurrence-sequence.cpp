#include "check/prelude.h"
const int mod = 1e9 + 7;
#include "code/linear-recurrence-sequence.cpp"

int main() {
	unsigned seed = 2; mt19937 rng(seed);
	for (int it = 0; it < 3000; ++it) {
		int k = rng() % 6 + 1, n = rng() % 300;
		vector<int> a(k), f(k);
		for (auto &x : a) x = rng() % (it % 3 ? 10 : mod);
		for (auto &x : f) x = rng() % (it % 3 ? 10 : mod);
		vector<ll> h(f.begin(), f.end());
		for (int i = k; i <= n; ++i) { ll s = 0; for (int j = 0; j < k; ++j) s = (s + (ll)a[j] * h[i - 1 - j]) % mod; h.push_back(s); }
		int got = solve(a, f, n);
		if (got != h[n]) { printf("seed %u it %d k %d n %d: want %lld got %d\n", seed, it, k, n, h[n], got); return 1; }
	}
	if (solve({1, 1}, {0, 1}, 90) != 2880067194370816120LL % mod) { puts("fib(90)"); return 1; }
	puts("OK linear-recurrence-sequence");
}
