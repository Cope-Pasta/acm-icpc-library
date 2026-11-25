#include "check/prelude.h"
#include "code/scc.cpp"

int main() {
	mt19937 rng(8);
	for (int it = 0; it < 20000; ++it) {
		int n = 1 + rng() % 9, m = rng() % 20;
		vector<vector<int>> adj(n);
		for (int i = 0; i < m; ++i) adj[rng() % n].push_back(rng() % n);
		vector<vector<bool>> reach(n, vector<bool>(n)); // Floyd-Warshall closure
		for (int u = 0; u < n; ++u) { reach[u][u] = true; for (int v : adj[u]) reach[u][v] = true; }
		for (int k = 0; k < n; ++k) for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if (reach[i][k] && reach[k][j]) reach[i][j] = true;
		vector<vector<int>> comps, cond;
		strongly_connected_components(adj, comps, cond);
		vector<int> id(n, -1), root(n);
		bool ok = true;
		for (int c = 0; c < (int)comps.size(); ++c) for (int v : comps[c]) {
			ok &= id[v] == -1, id[v] = c;
			root[v] = *min_element(comps[c].begin(), comps[c].end());
		}
		for (int i = 0; ok && i < n; ++i) for (int j = 0; ok && j < n; ++j)
			ok = id[i] != -1 && (id[i] == id[j]) == (reach[i][j] && reach[j][i]);
		set<pair<int, int>> want, got; // condensation edges between component roots
		for (int u = 0; ok && u < n; ++u) for (int v : adj[u]) if (root[u] != root[v]) want.insert({root[u], root[v]});
		for (int u = 0; ok && u < (int)cond.size(); ++u) for (int v : cond[u]) got.insert({u, v});
		if (!ok || want != got) {
			printf("mismatch on graph n=%d:", n);
			for (int u = 0; u < n; ++u) for (int v : adj[u]) printf(" %d->%d", u, v);
			puts(""); return 1;
		}
	}
	puts("OK scc");
}
