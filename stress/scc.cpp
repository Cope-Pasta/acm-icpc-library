#include "check/prelude.h"
#include "code/scc.cpp"
// brute: same SCC iff mutually reachable (Floyd-Warshall closure); ids must be 0..cnt-1 and reverse topological

int main() {
	mt19937 rng(8);
	for (int it = 0; it < 20000; ++it) {
		int n = 1 + rng() % 9, m = rng() % 20;
		vector<vector<int>> adj(n);
		for (int i = 0; i < m; ++i) adj[rng() % n].push_back(rng() % n);
		vector<vector<bool>> reach(n, vector<bool>(n));
		for (int u = 0; u < n; ++u) { reach[u][u] = true; for (int v : adj[u]) reach[u][v] = true; }
		for (int k = 0; k < n; ++k) for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if (reach[i][k] && reach[k][j]) reach[i][j] = true;
		SCC s(adj);
		bool ok = true; set<int> ids;
		for (int i = 0; i < n; ++i) ok &= 0 <= s.comp[i] && s.comp[i] < s.cnt, ids.insert(s.comp[i]);
		ok &= (int)ids.size() == s.cnt;
		for (int i = 0; ok && i < n; ++i) for (int j = 0; ok && j < n; ++j)
			ok = (s.comp[i] == s.comp[j]) == (reach[i][j] && reach[j][i]);
		for (int u = 0; ok && u < n; ++u) for (int v : adj[u]) ok &= s.comp[u] >= s.comp[v];
		if (!ok) {
			printf("mismatch on graph n=%d:", n);
			for (int u = 0; u < n; ++u) for (int v : adj[u]) printf(" %d->%d", u, v);
			puts(""); return 1;
		}
	}
	puts("OK scc");
}
