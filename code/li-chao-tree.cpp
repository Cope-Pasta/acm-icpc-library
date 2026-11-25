// Li Chao tree: min of lines k*x+b at integer x in [0, n), O(log n) each; LLONG_MAX if no lines.
// add({k, b}, 1, 0, n), get(x, 1, 0, n).
struct LiChao {
	struct L {
		ll k, b;
		ll f(ll x) { return k * x + b; }
	};
	vector<L> t;
	LiChao(int n) : t(4 * n, {0, LLONG_MAX}) {}
	void add(L a, int v, int l, int r) {
		int m = (l + r) / 2;
		bool lef = a.f(l) < t[v].f(l), mid = a.f(m) < t[v].f(m);
		if (mid) swap(a, t[v]);
		if (r - l > 1) lef != mid ? add(a, 2 * v, l, m) : add(a, 2 * v + 1, m, r);
	}
	ll get(int x, int v, int l, int r) {
		int m = (l + r) / 2;
		ll s = t[v].f(x);
		return r - l == 1 ? s : min(s, x < m ? get(x, 2 * v, l, m) : get(x, 2 * v + 1, m, r));
	}
};
