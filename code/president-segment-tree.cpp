// Persistent segment tree: range add, range sum over a[0..n-1], O(log n) per op (tags are never pushed).
// rt[0]: initial version. add(v, l, r, x): new version = version v plus x on [l, r]. ask(v, l, r): sum of [l, r] in version v.
struct PST {
	int n;
	vector<ll> s, d;
	vector<int> L, R, rt;
	int node(ll S, ll D, int l, int r) {
		s.push_back(S), d.push_back(D), L.push_back(l), R.push_back(r);
		return s.size() - 1;
	}
	int build(vector<int> &a, int l, int r) {
		if (l == r) return node(a[l], 0, 0, 0);
		int m = (l + r) / 2, x = build(a, l, m), y = build(a, m + 1, r);
		return node(s[x] + s[y], 0, x, y);
	}
	PST(vector<int> a) : n(a.size()) { rt = {build(a, 0, n - 1)}; }
	int upd(int t, int l, int r, int ql, int qr, ll x) {
		if (qr < l || r < ql) return t;
		int u = node(s[t] + x * (min(r, qr) - max(l, ql) + 1), d[t], L[t], R[t]), m = (l + r) / 2;
		if (ql <= l && r <= qr) d[u] += x;
		else L[u] = upd(L[t], l, m, ql, qr, x), R[u] = upd(R[t], m + 1, r, ql, qr, x);
		return u;
	}
	ll qry(int t, int l, int r, int ql, int qr) {
		if (qr < l || r < ql) return 0;
		if (ql <= l && r <= qr) return s[t];
		int m = (l + r) / 2;
		return d[t] * (min(r, qr) - max(l, ql) + 1) + qry(L[t], l, m, ql, qr) + qry(R[t], m + 1, r, ql, qr);
	}
	void add(int v, int l, int r, ll x) { rt.push_back(upd(rt[v], 0, n - 1, l, r, x)); }
	ll ask(int v, int l, int r) { return qry(rt[v], 0, n - 1, l, r); }
};
