#include "check/prelude.h"
#include "code/k-d-tree.cpp"
// Brute force: sort all squared distances to q and compare with the k found by knn().
ll dist2(const P &a, const P &b) { ll s = 0; for (size_t i = 0; i < a.size(); ++i) s += (a[i] - b[i]) * (a[i] - b[i]); return s; }
int main() {
	mt19937 gen(6);
	for (int it = 0; it < 3000; ++it) {
		int d = gen() % 5 + 1, n = gen() % 40 + 1, range = gen() % 2 ? 5 : 100; // small range -> many ties/duplicates
		vector<P> points(n, P(d));
		for (auto &p : points) for (auto &x : p) x = gen() % range;
		KD t(points);
		vector<P> a = t.p, b = points; sort(all(a)), sort(all(b));
		if (a != b) { printf("points changed it=%d\n", it); return 1; }
		for (int qi = 0; qi < 20; ++qi) {
			P q(d);
			for (auto &x : q) x = gen() % range;
			int k = gen() % n + 1;
			auto pq = t.knn(q, k);
			vector<ll> got, want; set<int> ids;
			for (; !pq.empty(); pq.pop()) {
				auto [s, i] = pq.top();
				if (dist2(t.p[i], q) != s) { printf("wrong index it=%d q=%d\n", it, qi); return 1; }
				got.push_back(s), ids.insert(i);
			}
			for (auto &p : points) want.push_back(dist2(p, q));
			sort(all(got)), sort(all(want)), want.resize(k);
			if (got != want || (int)ids.size() != k) { printf("mismatch it=%d q=%d n=%d d=%d k=%d\n", it, qi, n, d, k); return 1; }
		}
	}
	puts("OK k-d-tree");
}
