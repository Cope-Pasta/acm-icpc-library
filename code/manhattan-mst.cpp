// Manhattan MST, O(n log n): returns <= 4n candidate edges {w, i, j} (point ids, 0-indexed) that contain
// an MST of the points p (x, y) under |dx| + |dy|; run Kruskal on them.
vector<array<int, 3>> manhattan(vector<pair<int, int>> p) {
	vector<int> id(p.size());
	iota(all(id), 0);
	vector<array<int, 3>> e;
	for (int k = 0; k < 4; k++) {
		sort(all(id), [&](int i, int j) { return p[i].first - p[j].first < p[j].second - p[i].second; });
		map<int, int> sw;
		for (int i : id) {
			for (auto it = sw.lower_bound(-p[i].second); it != sw.end(); sw.erase(it++)) {
				int j = it->second, dx = p[i].first - p[j].first, dy = p[i].second - p[j].second;
				if (dy > dx) break;
				e.push_back({dx + dy, i, j});
			}
			sw[-p[i].second] = i;
		}
		for (auto &[x, y] : p)
			if (k & 1) x = -x;
			else swap(x, y);
	}
	return e;
}
