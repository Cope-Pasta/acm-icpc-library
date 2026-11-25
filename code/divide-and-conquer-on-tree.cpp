vector<vector<int>> edge; // vertices 0..n-1
vector<int> sz, parent, ver; // sz.assign(n, 0), parent.assign(n, -1)
vector<bool> cut; // cut.assign(n, false)

void bfs(int r) { // bfs in each sub-tree
	parent[r] = -1, ver.clear(); // maintain root extra information
	static queue<int> Q; static stack<int> U;
	Q.push(r);
	while (!Q.empty()) {
		int u = Q.front();
		Q.pop(); U.push(u);
		sz[u] = 1, ver.push_back(u); // find a node in sub-tree
		for (int v : edge[u]) {
			if (v == parent[u] || cut[v]) continue;
			parent[v] = u; // maintain v from u
			Q.push(v);
		}
	}
	while (!U.empty()) {
		int u = U.top(); U.pop();
		if (parent[u] != -1) sz[parent[u]] += sz[u];
	}
}

int findCentre(int r) {
	static queue<int> Q;
	int result = -1, rsize = maxint;
	bfs(r);
	Q.push(r);
	while (!Q.empty()) {
		int u = Q.front();
		Q.pop();
		int temp = sz[r] - sz[u];
		for (int v : edge[u]) {
			if (cut[v] || v == parent[u]) continue;
			temp = max(temp, sz[v]);
			Q.push(v);
		}
		if (temp < rsize) rsize = temp, result = u;
	}
	return result;
}

int work(int u) {
	int result = 0;
	u = findCentre(u);
	cut[u] = true;
	for (int v : edge[u]) {
		if (!cut[v]) /*result += */work(v); // process each sub-tree
	}
	for (int v : edge[u]) {
		if (cut[v]) continue;
		bfs(v); // then combine sub-trees
	}
	cut[u] = false;
	return result;
}
