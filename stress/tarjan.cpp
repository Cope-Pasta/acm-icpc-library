#include "check/prelude.h"
#include "code/tarjan.cpp"
// directed: the SCC block (uncommented copy below) vs mutual reachability (Floyd-Warshall closure)
// undirected: cut vertices from the snippet's low[] vs brute vertex removal
mt19937 rng(9); int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
void scc(int u) { // the commented-out SCC block of the snippet, enabled
	dfn[u] = low[u] = ++ind;
	sk.push(u), instack[u] = true;
	for (int v : edge[u]) {
		if (!dfn[v]) scc(v), low[u] = min(low[u], low[v]);
		else if (instack[v]) low[u] = min(low[u], dfn[v]);
	}
	if (dfn[u] == low[u]) {
		++color; int v;
		do { v = sk.top(), instack[v] = false; sk.pop(); block[v] = color; } while (v != u);
	}
}
int main() {
	for (int it = 0; it < 20000; ++it) {
		int n = rnd(1, 9); edge.assign(n, {});
		vector<vector<bool>> R(n, vector<bool>(n));
		for (int k = rnd(0, 20); k--;) { int u = rnd(0, n - 1), v = rnd(0, n - 1); edge[u].push_back(v), R[u][v] = true; }
		for (int i = 0; i < n; ++i) R[i][i] = true;
		for (int k = 0; k < n; ++k) for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if (R[i][k] && R[k][j]) R[i][j] = true;
		ind = 0, dfn.assign(n, 0), low.assign(n, 0), instack.assign(n, false), sk = {};
		for (int i = 0; i < n; ++i) if (!dfn[i]) tarjan(i);
		vector<int> d1 = dfn;
		ind = color = 0, dfn.assign(n, 0), low.assign(n, 0), block.assign(n, 0), instack.assign(n, false), sk = {};
		for (int i = 0; i < n; ++i) if (!dfn[i]) scc(i);
		bool ok = d1 == dfn;
		for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) ok &= (block[i] == block[j]) == (R[i][j] && R[j][i]);
		if (!ok) { printf("FAIL it=%d n=%d (scc)\n", it, n); return 1; }
	}
	for (int it = 0; it < 20000; ++it) { // undirected, no self-loops
		int n = rnd(1, 9); edge.assign(n, {});
		for (int k = rnd(0, 15); k--;) { int u = rnd(0, n - 1), v = rnd(0, n - 1); if (u != v) edge[u].push_back(v), edge[v].push_back(u); }
		ind = 0, dfn.assign(n, 0), low.assign(n, 0), instack.assign(n, false), sk = {};
		vector<bool> isRoot(n);
		for (int i = 0; i < n; ++i) if (!dfn[i]) isRoot[i] = true, tarjan(i);
		auto comps = [&](int ban) {
			vector<bool> vis(n); int c = 0;
			for (int s = 0; s < n; ++s) if (s != ban && !vis[s]) {
				++c; vector<int> st{s}; vis[s] = true;
				while (!st.empty()) { int u = st.back(); st.pop_back(); for (int v : edge[u]) if (v != ban && !vis[v]) vis[v] = true, st.push_back(v); }
			}
			return c;
		};
		int base = comps(-1);
		for (int u = 0; u < n; ++u) {
			int children = 0; bool cutv = false; // tree parent of v: neighbour with largest dfn below dfn[v]
			for (int v = 0; v < n; ++v) {
				int par = -1; for (int w : edge[v]) if (dfn[w] < dfn[v] && (par == -1 || dfn[w] > dfn[par])) par = w;
				if (par == u && !isRoot[v]) ++children, cutv |= low[v] >= dfn[u];
			}
			if (isRoot[u]) cutv = children >= 2;
			if (cutv != (comps(u) > base - (edge[u].empty() ? 1 : 0))) { printf("FAIL it=%d n=%d u=%d (cut vertex)\n", it, n, u); return 1; }
		}
	}
	puts("OK tarjan");
}
