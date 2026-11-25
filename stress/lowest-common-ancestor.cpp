#include "check/prelude.h"
#include "code/lowest-common-ancestor.cpp"
// brute: walk the deeper vertex up until both meet
mt19937 rng(10); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
int main() {
	for (int it = 0; it < 3000; ++it) {
		int n = rnd(1, it % 10 ? 30 : 3000); vector<int> perm(n); iota(perm.begin(), perm.end(), 0); shuffle(perm.begin(), perm.end(), rng);
		vector<int> parent(n), depth(n);
		parent[perm[0]] = perm[0];
		for (int i = 1; i < n; ++i) { // chains sometimes, to get deep trees
			int p = perm[rnd(0, 3) ? i - 1 : rnd(0, i - 1)];
			parent[perm[i]] = p, depth[perm[i]] = depth[p] + 1;
		}
		LCA lca(parent, depth);
		for (int q = 0; q < 50; ++q) {
			int x = rnd(0, n - 1), y = rnd(0, n - 1), a = x, b = y;
			while (a != b) depth[a] >= depth[b] ? a = parent[a] : b = parent[b];
			int r = lca.query(x, y);
			if (r != a) { printf("FAIL it=%d n=%d: LCA(%d,%d) = %d, brute %d\n", it, n, x, y, r, a); return 1; }
		}
	}
	puts("OK lowest-common-ancestor");
}
