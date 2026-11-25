#include "check/prelude.h"
#include "code/link-cut-tree.cpp"
// Brute force: a rooted forest as a parent array. After access(x) + splay(x), x's splay
// tree holds exactly the root-to-x path, so s[x].size - 1 must be the depth of x.
int main() {
	mt19937 rng(2); long long checks = 0;
	for (int it = 0; it < 3000; ++it) {
		int n = rng() % 30 + 1;
		init(n);
		vector<int> par(n, -1);
		auto depth = [&](int x) { int d = 0; while (par[x] != -1) x = par[x], ++d; return d; };
		auto anc = [&](int a, int x) { for (; x != -1; x = par[x]) if (x == a) return true; return false; };
		for (int op = 0; op < 100; ++op) {
			int ty = rng() % 3, x = rng() % n, y = rng() % n;
			if (ty == 0) { // link root y under x
				if (par[y] != -1 || anc(y, x)) continue;
				par[y] = x, link(&s[x], &s[y]);
			} else if (ty == 1) { // cut x from its parent
				if (par[x] == -1) continue;
				par[x] = -1, cut(&s[x]);
			} else {
				access(&s[x]), splay(&s[x]), ++checks;
				if (s[x].size - 1 != depth(x)) { printf("depth mismatch it=%d op=%d x=%d: %d vs %d\n", it, op, x, s[x].size - 1, depth(x)); return 1; }
			}
		}
	}
	printf("OK link-cut-tree (%lld checks)\n", checks);
}
