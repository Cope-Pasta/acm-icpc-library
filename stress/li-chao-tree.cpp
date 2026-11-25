#include "check/prelude.h"
#include "code/li-chao-tree.cpp"
const int maxn = 200000;
// Brute force: min over all added lines k*X+b (no lines -> LLONG_MAX).
int main() {
	mt19937 gen(7);
	for (int it = 0; it < 150; ++it) {
		LiChao t(maxn);
		vector<pair<ll, ll>> ls;
		int span = it % 3 == 0 ? maxn : 50; // queries over the whole range or near 0
		for (int op = 0; op < 300; ++op) {
			if (gen() % 2) {
				ll k = ll(gen() % 2000001) - 1000000, b = ll(gen() % 2000000001) - 1000000000;
				if (gen() % 4 == 0 && !ls.empty()) k = ls[gen() % ls.size()].first; // parallel lines
				t.add({k, b}, 1, 0, maxn), ls.push_back({k, b});
			} else {
				int X = gen() % span;
				ll want = LLONG_MAX;
				for (auto &l : ls) want = min(want, l.first * X + l.second);
				if (t.get(X, 1, 0, maxn) != want) { printf("mismatch it=%d op=%d X=%d: %lld vs %lld\n", it, op, X, t.get(X, 1, 0, maxn), want); return 1; }
			}
		}
	}
	puts("OK li-chao-tree");
}
