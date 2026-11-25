#include "check/prelude.h"
#include "code/president-segment-tree.cpp"
// Brute force: every version kept as a plain array; range add creates a new version.
int main() {
	mt19937 gen(5);
	for (int it = 0; it < 1000; ++it) {
		int n = gen() % 20 + 1;
		a.assign(n, 0);
		for (int &x : a) x = int(gen() % 21) - 10;
		init();
		vector<vector<int>> b = {a};
		for (int op = 0; op < 100; ++op) {
			int v = gen() % b.size(), l = gen() % n, r = l + gen() % (n - l);
			if (gen() % 2) { // add d on [l, r] of version v -> new version
				int d = int(gen() % 11) - 5;
				root.push_back(add(root[v], 0, n - 1, l, r, d));
				b.push_back(b[v]);
				for (int i = l; i <= r; ++i) b.back()[i] += d;
			} else { // sum of [l, r] in version v
				int want = accumulate(b[v].begin() + l, b[v].begin() + r + 1, 0);
				if (ask(root[v], 0, n - 1, l, r) != want) { printf("mismatch it=%d op=%d version=%d [%d, %d]\n", it, op, v, l, r); return 1; }
			}
		}
	}
	puts("OK president-segment-tree");
}
