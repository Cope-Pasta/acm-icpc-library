// Lengauer-Tarjan dominator tree, O(m log n). g: directed adjacency list, 0-indexed.
// dom[v] = immediate dominator of v, dom[root] = root, -1 if v is unreachable from root.
struct Dominator {
	vector<int> dom, semi, anc, best; // semi/anc/best indexed by dfs number
	int eval(int v) {
		int a = anc[v];
		if (a < 0) return v;
		if (anc[a] >= 0) {
			if (semi[eval(a)] < semi[best[v]]) best[v] = best[a];
			anc[v] = anc[a];
		}
		return best[v];
	}
	Dominator(const vector<vector<int>> &g, int root) : dom(g.size(), -1) {
		int n = g.size();
		vector<int> id(n, -1), rev, par(n), idom(n);
		vector<vector<int>> pred(n), bkt(n);
		function<void(int)> dfs = [&](int u) {
			id[u] = rev.size(), rev.push_back(u);
			for (int v : g[u])
				if (id[v] < 0) dfs(v), par[id[v]] = id[u];
		};
		dfs(root);
		for (int u : rev)
			for (int v : g[u]) pred[id[v]].push_back(id[u]);
		int k = rev.size();
		semi.resize(k), anc.assign(k, -1), iota(all(semi), 0), best = semi;
		for (int w = k - 1; w > 0; w--) {
			for (int v : pred[w]) semi[w] = min(semi[w], semi[eval(v)]);
			bkt[semi[w]].push_back(w);
			int p = anc[w] = par[w];
			for (int v : bkt[p]) idom[v] = semi[eval(v)] < p ? eval(v) : p;
			bkt[p].clear();
		}
		for (int w = 1; w < k; w++) {
			if (idom[w] != semi[w]) idom[w] = idom[idom[w]];
			dom[rev[w]] = rev[idom[w]];
		}
		dom[root] = root;
	}
};
