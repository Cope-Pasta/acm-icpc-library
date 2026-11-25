// Tarjan SCC, O(n+m). g: adjacency list, vertices 0..n-1. comp[v] = SCC id in 0..cnt-1;
// ids are reverse topological (edge u->v => comp[u] >= comp[v]); condensation: comp[u]->comp[v].
struct SCC {
	vector<int> val, comp, z;
	int t = 0, cnt = 0;
	SCC(const vector<vector<int>> &g) : val(g.size()), comp(g.size(), -1) {
		for (int i = 0; i < (int)g.size(); i++)
			if (comp[i] < 0) dfs(i, g);
	}
	int dfs(int u, const vector<vector<int>> &g) {
		int low = val[u] = ++t, x;
		z.push_back(u);
		for (int v : g[u])
			if (comp[v] < 0) low = min(low, val[v] ? val[v] : dfs(v, g));
		if (low == val[u]) {
			do x = z.back(), z.pop_back(), comp[x] = cnt;
			while (x != u);
			cnt++;
		}
		return val[u] = low;
	}
};
