#include "check/prelude.h"
#include "code/link-cut-tree.cpp"
// Brute force: a rooted forest as a parent array. After access(x), x's splay tree holds
// exactly the root-to-x path, so sz[x] - 1 must be the depth of x; access(u), access(v) gives the lca.
int main() {
	mt19937 rng(2); long long checks = 0;
	for (int it = 0; it < 3000; ++it) {
		int n = rng() % 30 + 1;
		LCT t(n);
		vector<int> par(n, -1);
		auto depth = [&](int x) { int d = 0; while (par[x] != -1) x = par[x], ++d; return d; };
		auto anc = [&](int a, int x) { for (; x != -1; x = par[x]) if (x == a) return true; return false; };
		for (int op = 0; op < 100; ++op) {
			int ty = rng() % 4, x = rng() % n, y = rng() % n;
			if (ty == 0) { // link root y under x
				if (par[y] != -1 || anc(y, x)) continue;
				par[y] = x, t.link(x, y);
			} else if (ty == 1) { // cut x from its parent
				if (par[x] == -1) continue;
				par[x] = -1, t.cut(x);
			} else if (ty == 2) {
				t.access(x), ++checks;
				if (t.sz[x] - 1 != depth(x)) { printf("depth mismatch it=%d op=%d x=%d: %d vs %d\n", it, op, x, t.sz[x] - 1, depth(x)); return 1; }
			} else { // lca of x and y, if in the same tree
				int l = x;
				while (!anc(l, y) && par[l] != -1) l = par[l];
				if (!anc(l, y)) continue;
				t.access(x), ++checks;
				int got = t.access(y);
				if (got != l) { printf("lca mismatch it=%d op=%d x=%d y=%d: %d vs %d\n", it, op, x, y, got, l); return 1; }
			}
		}
	}
	printf("OK link-cut-tree (%lld checks)\n", checks);
}
