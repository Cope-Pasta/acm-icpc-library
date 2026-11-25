int n; // maximum matching on graphs, vertices 0..n-1, match[u] = -1 if unmatched
vector<int> match, pre, base;
vector<vector<int>> edge; // edge.assign(n, {})
vector<bool> inQ, inB, inP;
queue<int> Q;

int LCA(int u, int v) {
	inP.assign(n, false);
	while (true) {
		u = base[u];
		inP[u] = true;
		if (match[u] == -1) break;
		u = pre[match[u]];
	}
	while (true) {
		v = base[v];
		if (inP[v]) return v;
		v = pre[match[v]];
	}
}

void reset(int u, int a) {
	while (u != a) {
		int v = match[u];
		inB[base[u]] = inB[base[v]] = true;
		v = pre[v];
		if (base[v] != a) pre[v] = match[u];
		u = v;
	}
}

void contract(int u, int v) {
	int a = LCA(u, v);
	inB.assign(n, false);
	reset(u, a), reset(v, a);
	if (base[u] != a) pre[u] = v;
	if (base[v] != a) pre[v] = u;
	for (int i = 0; i < n; ++i) {
		if (!inB[base[i]]) continue;
		base[i] = a;
		if (!inQ[i]) Q.push(i), inQ[i] = true;
	}
}

bool dfs(int s) {
	pre.assign(n, -1), inQ.assign(n, false), base.resize(n);
	iota(base.begin(), base.end(), 0);
	while (!Q.empty()) Q.pop();
	Q.push(s), inQ[s] = true;
	while (!Q.empty()) {
		int u = Q.front();
		Q.pop();
		for (int v : edge[u]) {
			if (base[u] == base[v] || match[u] == v) continue;
			if (v == s || (match[v] != -1 && pre[match[v]] != -1)) contract(u, v);
			else if (pre[v] == -1) {
				pre[v] = u;
				if (match[v] != -1) {
					Q.push(match[v]), inQ[match[v]] = true;
				} else {
					u = v;
					while (u != -1) {
						v = pre[u];
						int w = match[v];
						match[u] = v, match[v] = u;
						u = w;
					}
					return true;
				}
			}
		}
	}
	return false;
}

int blossom() {
	int ans = 0;
	match.assign(n, -1);
	for (int i = 0; i < n; ++i) {
		if (match[i] == -1 && dfs(i)) ++ans;
	}
	return ans;
}