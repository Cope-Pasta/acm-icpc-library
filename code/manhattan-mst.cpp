struct point {
	int x, y, index;
	bool operator<(const point &p) const { return x == p.x ? y < p.y : x < p.x; }
};
struct node {
	int value, p;
};
vector<node> T; // Fenwick tree, 1-indexed

int query(int x) {
	int r = maxint, p = -1;
	for (; x < int(T.size()); x += (x & -x)) if (T[x].value < r) r = T[x].value, p = T[x].p;
	return p;
}

void modify(int x, int w, int p) {
	for (; x > 0; x -= (x & -x)) if (T[x].value > w) T[x].value = w, T[x].p = p;
}

int manhattan(vector<point> p) { // add(u, v, w) gets 0-indexed point ids
	int n = p.size();
	for (int i = 0; i < n; ++i) p[i].index = i;
	for (int dir = 0; dir < 4; ++dir) {
		if (dir == 1 || dir == 3) {
			for (auto &q : p) swap(q.x, q.y);
		} else if (dir == 2) {
			for (auto &q : p) q.x = -q.x;
		}
		sort(p.begin(), p.end());
		vector<int> v, a(n);
		for (int i = 0; i < n; ++i) a[i] = p[i].y - p[i].x, v.push_back(a[i]);
		sort(v.begin(), v.end()); v.erase(unique(v.begin(), v.end()), v.end());
		for (int i = 0; i < n; ++i) a[i] = lower_bound(v.begin(), v.end(), a[i]) - v.begin() + 1;
		T.assign(n + 1, {maxint, -1});
		for (int i = n - 1; i >= 0; --i) {
			int pos = query(a[i]);
			if (pos != -1) add(p[i].index, p[pos].index, dist(p[i], p[pos]));
			modify(a[i], p[i].x + p[i].y, i);
		}
	}
	return kruskal();
}