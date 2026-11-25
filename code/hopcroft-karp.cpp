int n, m; // Left: 0..n-1, Right: 0..m-1
vector<int> vis, level, pr, pr2;
vector<vector<int>> edge; // for Left, edge.assign(n, {})

bool dfs(int u) {
	vis[u] = true;
	for (int w : edge[u]) {
		int v = pr2[w];
		if (v == -1 || (!vis[v] && level[u] < level[v] && dfs(v))) {
			pr[u] = w, pr2[w] = u;
			return true;
		}
	}
	return false;
}

int hopcroftKarp() {
	pr.assign(n, -1), pr2.assign(m, -1), level.assign(n, 0);
	for (int match = 0; ;) {
		queue<int> Q;
		for (int i = 0; i < n; ++i) {
			if (pr[i] == -1) {
				level[i] = 0;
				Q.push(i);
			} else level[i] = -1;
		}
		while (!Q.empty()) {
			int u = Q.front(); Q.pop();
			for (int w : edge[u]) {
				int v = pr2[w];
				if (v != -1 && level[v] < 0) {
					level[v] = level[u] + 1;
					Q.push(v);
				}
			}
		}
		vis.assign(n, false);
		int d = 0;
		for (int i = 0; i < n; ++i) if (pr[i] == -1 && dfs(i)) ++d;
		if (d == 0) return match;
		match += d;
	}
}
