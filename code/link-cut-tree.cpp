// Link-cut tree on a rooted forest of nodes 0..n-1 (n is nil), amortized O(log n) per op.
// link(x, y): hang root y under x. cut(x): cut x from its parent.
// access(x): x's splay tree becomes the root..x path, x on top, so sz[x] = depth(x) + 1.
// access(u) then access(v) returns lca(u, v) (same tree).
struct LCT {
	int n;
	vector<int> p, sz;
	vector<array<int, 2>> ch;
	LCT(int n) : n(n), p(n + 1, n), sz(n + 1, 1), ch(n + 1, {n, n}) { sz[n] = 0; }
	bool nr(int x) { return ch[p[x]][0] == x || ch[p[x]][1] == x; } // not a splay root
	void pull(int x) { sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + 1; }
	void rot(int x) {
		int y = p[x], z = p[y], d = ch[y][1] == x, b = ch[x][!d];
		if (nr(y)) ch[z][ch[z][1] == y] = x;
		p[x] = z, ch[y][d] = b, p[b] = y, ch[x][!d] = y, p[y] = x;
		pull(y), pull(x);
	}
	void splay(int x) {
		for (int y; nr(x); rot(x))
			if (y = p[x], nr(y)) rot((ch[y][1] == x) == (ch[p[y]][1] == y) ? y : x);
	}
	int access(int x) {
		int l = x;
		for (int y = n, z = x; z != n; y = z, z = p[z]) splay(l = z), ch[z][1] = y, pull(z);
		splay(x);
		return l;
	}
	void link(int x, int y) { access(y), p[y] = x; }
	void cut(int x) { access(x), p[ch[x][0]] = n, ch[x][0] = n, pull(x); }
};
