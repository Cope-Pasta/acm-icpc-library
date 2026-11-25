#include "check/prelude.h"
#include "code/li-chao-tree.cpp"
// Brute force: min over all added lines k*X+b (no lines -> LLONG_MAX). The snippet
// defines x/y as macros, so no variables here are named x or y.
int main() {
	mt19937 gen(7);
	for (int it = 0; it < 150; ++it) {
		line.assign(4 * maxn, point(0, LLONG_MAX));
		vector<pair<ll, ll>> ls;
		int span = it % 3 == 0 ? maxn : 50; // queries over the whole range or near 0
		for (int op = 0; op < 300; ++op) {
			if (gen() % 2) {
				ll k = ll(gen() % 2000001) - 1000000, b = ll(gen() % 2000000001) - 1000000000;
				if (gen() % 4 == 0 && !ls.empty()) k = ls[gen() % ls.size()].first; // parallel lines
				add_line(point(k, b)), ls.push_back({k, b});
			} else {
				int X = gen() % span;
				ll want = LLONG_MAX;
				for (auto &l : ls) want = min(want, l.first * X + l.second);
				if (get(X) != want) { printf("mismatch it=%d op=%d X=%d: %lld vs %lld\n", it, op, X, get(X), want); return 1; }
			}
		}
	}
	puts("OK li-chao-tree");
}
