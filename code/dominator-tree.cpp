int n, cnt; // vertices 0..n-1, labels (dfs order) 0..cnt-1
vector<vector<int>> edge, pred, bucket; // edge.assign(n, {})
vector<int> parent, label, rev, semi, idom, ancestor, best;

void dfs(int u) {
	label[u] = cnt;
	rev[cnt++] = u;
	for (int v : edge[u]) {
		if (v == parent[u] || label[v] != -1) continue;
		parent[v] = u;
		dfs(v);
	}
}

void link(int v, int w) {
	ancestor[w] = v;
}

void compress(int v) {
	int a = ancestor[v];
	if (ancestor[a] == -1) return;
	compress(a);
	if (semi[best[v]] > semi[best[a]]) best[v] = best[a];
	ancestor[v] = ancestor[a];
}

int eval(int v) {
	if (ancestor[v] == -1) return v;
	compress(v);
	return best[v];
}

void dominator(int root) {
	label.assign(n, -1), parent.assign(n, -1), rev.assign(n, -1), cnt = 0;
	dfs(root);
	pred.assign(n, {}), bucket.assign(n, {});
	for (int u = 0; u < n; ++u) {
		for (int v : edge[u]) {
			if (label[u] != -1 && label[v] != -1) pred[label[v]].push_back(label[u]);
		}
	}
	semi.resize(n), best.resize(n), idom.assign(n, -1), ancestor.assign(n, -1);
	iota(all(semi), 0), iota(all(best), 0);
	for (int w = cnt - 1; w >= 1; --w) {
		int p = label[parent[rev[w]]];
		for (int v : pred[w]) {
			int u = eval(v);
			if (semi[w] > semi[u]) semi[w] = semi[u];
		}
		bucket[semi[w]].push_back(w);
		link(p, w);
		for (int v : bucket[p]) {
			int u = eval(v);
			idom[v] = (semi[u] < p ? u : p);
		}
		bucket[p].clear();
	}
	for (int w = 1; w < cnt; ++w) {
		if (idom[w] != semi[w]) idom[w] = idom[idom[w]];
	}
	// for i in [1, cnt): rev[idom[i]] is immediate dominator of rev[i]
}
