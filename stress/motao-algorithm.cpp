#include "check/prelude.h"
#include "code/motao-algorithm.cpp"
// distinct count on A[l..r] via Mo vs std::set; add() asserts the range never removes an absent position

int main() {
	unsigned seed = 11; mt19937 rng(seed);
	for (int it = 0; it < 2000; ++it) {
		int n = rng() % 60 + 1, q = rng() % 80, v = rng() % 8 + 1;
		vector<int> A(n), occ(v), in(n); int res = 0;
		for (auto &x : A) x = rng() % v;
		vector<pair<int, int>> Q(q);
		for (auto &[l, r] : Q) { l = rng() % n, r = rng() % n; if (l > r) swap(l, r); }
		auto ans = mo(n, Q, [&](int i, int d) {
			assert(0 <= i && i < n && in[i] == (d < 0)); in[i] ^= 1;
			int &c = occ[A[i]]; if (d > 0) res += c++ == 0; else res -= --c == 0;
		}, [&] { return res; });
		for (int i = 0; i < q; ++i) {
			auto [l, r] = Q[i];
			int want = set<int>(A.begin() + l, A.begin() + r + 1).size();
			if (ans[i] != want) { printf("seed %u it %d query %d [%d, %d]: want %d got %lld\n", seed, it, i, l, r, want, ans[i]); return 1; }
		}
	}
	puts("OK motao-algorithm");
}
