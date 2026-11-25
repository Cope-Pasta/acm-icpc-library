#include "check/prelude.h"
#include "code/k-d-tree.cpp"
// Brute force: sort all squared distances to sp and compare with the scnt found by search().
int main() {
	mt19937 gen(6);
	for (int it = 0; it < 3000; ++it) {
		d = gen() % DIM + 1;
		int n = gen() % 40 + 1, range = gen() % 2 ? 5 : 100; // small range -> many ties/duplicates
		points.assign(n, {});
		for (auto &p : points) for (int i = 0; i < d; ++i) p.x[i] = gen() % range;
		init();
		for (int q = 0; q < 20; ++q) {
			for (int i = 0; i < d; ++i) sp.x[i] = gen() % range;
			scnt = gen() % n + 1;
			pq = {}, search(root);
			vector<int> got, want;
			for (; !pq.empty(); pq.pop()) got.push_back(dist2(pq.top(), sp));
			for (auto &p : points) want.push_back(dist2(p, sp));
			sort(got.begin(), got.end()), sort(want.begin(), want.end()), want.resize(scnt);
			if (got != want) { printf("mismatch it=%d q=%d n=%d d=%d scnt=%d\n", it, q, n, d, scnt); return 1; }
		}
	}
	puts("OK k-d-tree");
}
