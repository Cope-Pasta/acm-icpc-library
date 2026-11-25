// 3D convex hull, O(n^2). n >= 4, not all coplanar; reorders p. Faces index into p, CCW seen from outside
// (volume(q, face) <= 0 for all q). Points inside or on the surface are skipped; coplanar faces are triangulated.
vector<array<int, 3>> hull3d(vector<point3_t> &p) {
	int n = p.size();
	for (int i = 1; i < n; ++i)
		if (dblcmp(dist(p[0], p[i]))) swap(p[1], p[i]);
	for (int i = 2; i < n; ++i)
		if (dblcmp(area(p[0], p[1], p[i]))) swap(p[2], p[i]);
	for (int i = 3; i < n; ++i)
		if (dblcmp(volume(p[0], p[1], p[2], p[i]))) swap(p[3], p[i]);
	auto vol = [&](int i, array<int, 3> t) { return dblcmp(volume(p[i], p[t[0]], p[t[1]], p[t[2]])); };
	vector<array<int, 3>> f;
	for (int i = 0; i < 4; ++i) {
		array<int, 3> t = {(i + 1) % 4, (i + 2) % 4, (i + 3) % 4};
		if (vol(i, t) > 0) swap(t[0], t[1]);
		f.push_back(t);
	}
	vector<vector<int>> vis(n, vector<int>(n)); // vis[a][b] == i: edge a->b is on a face visible from p[i]
	for (int i = 4; i < n; ++i) {
		vector<array<int, 3>> g, h; // kept, visible
		for (auto &t : f) (vol(i, t) > 0 ? h : g).push_back(t);
		for (auto &t : h)
			for (int k = 0; k < 3; ++k) vis[t[k]][t[(k + 1) % 3]] = i;
		for (auto &t : h)
			for (int k = 0; k < 3; ++k)
				if (vis[t[(k + 1) % 3]][t[k]] != i) g.push_back({t[k], t[(k + 1) % 3], i}); // horizon edge
		f = g;
	}
	return f;
}
