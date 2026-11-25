// Tarjan lowlink on an undirected graph (multi-edges, self-loops ok), O(n+m). g: adjacency list, 0-indexed.
// cut[v]: v is an articulation point; br: bridges (u, v). For SCC (directed) see scc.cpp.
struct Lowlink {
	vector<int> dfn, low;
	vector<bool> cut;
	vector<pair<int, int>> br;
	int t = 0;
	Lowlink(const vector<vector<int>> &g) : dfn(g.size(), -1), low(g.size()), cut(g.size()) {
		for (int i = 0; i < (int)g.size(); i++)
			if (dfn[i] < 0) dfs(i, -1, g);
	}
	void dfs(int u, int p, const vector<vector<int>> &g) {
		dfn[u] = low[u] = t++;
		int ch = 0;
		for (int v : g[u])
			if (v == p) p = -2; // skip one copy of the tree edge; p == -1 only at a root
			else if (dfn[v] >= 0) low[u] = min(low[u], dfn[v]);
			else {
				dfs(v, u, g), ch++, low[u] = min(low[u], low[v]);
				if (low[v] > dfn[u]) br.push_back({u, v});
				if (low[v] >= dfn[u] && (p != -1 || ch > 1)) cut[u] = true;
			}
	}
};
