// Splay tree over a sequence of nodes; node x holds cnt[x] units. Node 0 is nil. Amortized O(log n).
// Keep a sentinel node at each end. add(c): new node (root = add(c) for the first one).
// ins(y, x, d): put x right after (d=1) / before (d=0) y. nb(x, d): next/previous node.
// find(k): node holding unit k (0-indexed). split(k): make unit k its own node, return it.
// expose(x, y): subtree holding exactly nodes x..y. cut(x, y): remove nodes x..y.
struct Splay {
	vector<int> p, sz, cnt;
	vector<array<int, 2>> ch;
	int root = 0;
	Splay() { add(0); }
	int add(int c) {
		p.push_back(0), sz.push_back(c), cnt.push_back(c), ch.push_back({0, 0});
		return p.size() - 1;
	}
	int dir(int x) { return ch[p[x]][1] == x; }
	void pull(int x) { sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + cnt[x]; }
	void rot(int x) {
		int y = p[x], z = p[y], d = dir(x), b = ch[x][!d];
		if (z) ch[z][dir(y)] = x;
		else root = x;
		p[x] = z, ch[y][d] = b, p[b] = y, ch[x][!d] = y, p[y] = x;
		pull(y), pull(x);
	}
	int splay(int x, int g = 0) {
		for (int y; (y = p[x]) != g; rot(x))
			if (p[y] != g) rot(dir(x) == dir(y) ? y : x);
		return x;
	}
	int nb(int x, int d) {
		for (x = ch[splay(x)][d]; ch[x][!d];) x = ch[x][!d];
		return x;
	}
	void ins(int y, int x, int d = 1) {
		if (ch[splay(y)][d]) y = nb(y, d), d = !d;
		ch[y][d] = x, p[x] = y, splay(x);
	}
	int find(int k) {
		for (int t = root;;) {
			int l = sz[ch[t][0]];
			if (k < l) t = ch[t][0];
			else if ((k -= l) < cnt[t]) return splay(t);
			else k -= cnt[t], t = ch[t][1];
		}
	}
	int split(int k) {
		int t = find(k), a = k - sz[ch[t][0]], b = cnt[t] - a - 1;
		cnt[t] = 1;
		if (a) ins(t, add(a), 0);
		if (b) ins(t, add(b));
		return t;
	}
	int expose(int x, int y) {
		x = nb(x, 0), y = nb(y, 1);
		return ch[splay(y, splay(x))][0];
	}
	void cut(int x, int y) {
		int t = expose(x, y), b = p[t];
		ch[b][0] = p[t] = 0, pull(b), pull(p[b]);
	}
};
