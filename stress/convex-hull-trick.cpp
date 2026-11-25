#include "check/prelude.h"
#include "code/convex-hull-trick.cpp"
// Brute force: min over all added lines k*X+b, lines added with strictly increasing k (as documented).
// The snippet defines x/y as macros, so no variables here are named x or y.
int main() {
	mt19937 gen(8);
	for (int it = 0; it < 5000; ++it) {
		hull.clear(), vecs.clear();
		int n = gen() % 30 + 1;
		vector<int> ks(n);
		for (int &k : ks) k = int(gen() % 201) - 100;
		sort(ks.begin(), ks.end()), ks.erase(unique(ks.begin(), ks.end()), ks.end());
		vector<pair<int, int>> ls;
		for (int k : ks) {
			int b = int(gen() % 2001) - 1000;
			add_line(k, b), ls.push_back({k, b});
			for (int q = 0; q < 5; ++q) {
				int X = int(gen() % 201) - 100, want = INT_MAX;
				for (auto &l : ls) want = min(want, l.first * X + l.second);
				if (get(X) != want) { printf("mismatch it=%d X=%d: %d vs %d\n", it, X, get(X), want); return 1; }
			}
		}
	}
	puts("OK convex-hull-trick");
}
