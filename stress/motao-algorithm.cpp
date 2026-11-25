#include "check/prelude.h"
struct query_t { int l, r, ID; };
bool cmpL(const query_t &a, const query_t &b) { return a.l < b.l; }
bool cmpR(const query_t &a, const query_t &b) { return a.r < b.r; }
vector<int> A, occ, ans; int res; // distinct count on A[l..r]
void clear() { fill(occ.begin(), occ.end(), 0), res = 0; }
void add(int pos, int d) { int &c = occ[A[pos]]; if (d > 0) res += c++ == 0; else res -= --c == 0; }
void motao(vector<query_t> Q) { // the snippet is a fragment: run it inside a function
#include "code/motao-algorithm.cpp"
}

int main() {
	unsigned seed = 11; mt19937 rng(seed);
	for (int it = 0; it < 2000; ++it) {
		int n = rng() % 60 + 1, q = rng() % 80, v = rng() % 8 + 1;
		A.resize(n); for (auto &x : A) x = rng() % v;
		occ.assign(v, 0);
		vector<query_t> Q(q);
		for (int i = 0; i < q; ++i) { int l = rng() % n, r = rng() % n; if (l > r) swap(l, r); Q[i] = {l, r, i}; }
		ans.assign(q, -1); motao(Q);
		for (int i = 0; i < q; ++i) {
			int want = set<int>(A.begin() + Q[i].l, A.begin() + Q[i].r + 1).size();
			if (ans[i] != want) { printf("seed %u it %d query %d [%d, %d]: want %d got %d\n", seed, it, i, Q[i].l, Q[i].r, want, ans[i]); return 1; }
		}
	}
	puts("OK motao-algorithm");
}
