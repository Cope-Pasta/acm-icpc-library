// Kuhn-Munkras's algorithm, n: Left Size <= m: Right Size, g: n*m, 0-indexed.
int n, m;
vector<vector<int>> g;
vector<int> lx, ly, slack, match; // match[y]: left vertex matched to y
vector<bool> vx, vy;

bool find(int x) {
	vx[x] = true;
	for (int y = 0; y < m; ++y) {
		if (!vy[y]) {
			int delta = lx[x] + ly[y] - g[x][y];
			if (delta == 0) {
				vy[y] = true;
				if (match[y] == -1 || find(match[y])) {
					match[y] = x;
					return true;
				}
			} else slack[y] = min(slack[y], delta);
		}
	}
	return false;
}

int km() { // maximum weight, if minimum, negate all g then restore at the end.
	lx.assign(n, INT_MIN), ly.assign(m, 0), match.assign(m, -1);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) lx[i] = max(lx[i], g[i][j]);
	}
	for (int k = 0; k < n; ++k) {
		slack.assign(m, maxint);
		while (true) {
			vx.assign(n, false), vy.assign(m, false);
			if (find(k)) break;
			else {
				int delta = maxint;
				for (int i = 0; i < m; ++i) {
					if (!vy[i]) delta = min(delta, slack[i]);
				}
				for (int i = 0; i < n; ++i) {
					if (vx[i]) lx[i] -= delta;
				}
				for (int i = 0; i < m; ++i) {
					if (vy[i]) ly[i] += delta;
					if (!vy[i]) slack[i] -= delta;
				}
			}
		}
	}
	return accumulate(lx.begin(), lx.end(), 0) + accumulate(ly.begin(), ly.end(), 0);
}
