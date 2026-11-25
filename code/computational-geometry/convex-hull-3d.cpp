struct face_t {
	int a, b, c;
	bool vis;
};
vector<point3_t> pt; // input; hull3d() reorders it, faces in fc index into pt
vector<vector<int>> bf;
vector<face_t> fc;

bool remove(int p, int b, int a) {
	int f = bf[b][a];
	if (fc[f].vis) {
		if (dblcmp(volume(pt[p], pt[fc[f].a], pt[fc[f].b], pt[fc[f].c])) >= 0) return true;
		bf[a][b] = bf[b][p] = bf[p][a] = fc.size();
		fc.push_back({a, b, p, true});
	}
	return false;
}

void dfs(int p, int f) {
	fc[f].vis = false;
	if (remove(p, fc[f].b, fc[f].a)) dfs(p, bf[fc[f].b][fc[f].a]);
	if (remove(p, fc[f].c, fc[f].b)) dfs(p, bf[fc[f].c][fc[f].b]);
	if (remove(p, fc[f].a, fc[f].c)) dfs(p, bf[fc[f].a][fc[f].c]);
}

void hull3d() { // n >= 4, not all coplanar
	int n = pt.size();
	for (int i = 1; i < n; ++i) {
		if (dblcmp((pt[i] - pt[0]).length()) > 0) swap(pt[i], pt[1]);
	}
	for (int i = 2; i < n; ++i) {
		if (dblcmp(fabs(area(pt[0], pt[1], pt[i]))) > 0) swap(pt[i], pt[2]);
	}
	for (int i = 3; i < n; ++i) {
		if (dblcmp(fabs(volume(pt[0], pt[1], pt[2], pt[i]))) > 0) swap(pt[i], pt[3]);
	}
	fc.clear(), bf.assign(n, vector<int>(n));
	for (int i = 0; i < 4; ++i) {
		face_t f = {(i + 1) % 4, (i + 2) % 4, (i + 3) % 4, true};
		if (dblcmp(volume(pt[i], pt[f.a], pt[f.b], pt[f.c])) > 0) swap(f.a, f.b);
		bf[f.a][f.b] = bf[f.b][f.c] = bf[f.c][f.a] = fc.size();
		fc.push_back(f);
	}
	shuffle(pt.begin() + 4, pt.end(), mt19937(time(0)));
	for (int i = 4; i < n; ++i) {
		for (int j = 0; j < int(fc.size()); ++j) {
			if (!fc[j].vis) continue;
			if (dblcmp(volume(pt[i], pt[fc[j].a], pt[fc[j].b], pt[fc[j].c])) >= 0) {
				dfs(i, j);
				break;
			}
		}
	}
	fc.erase(remove_if(all(fc), [](const face_t &f) { return !f.vis; }), fc.end());
}