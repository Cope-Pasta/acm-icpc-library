// Stoer-Wagner global min cut, O(n^3). g: n*n symmetric weight matrix, 0-indexed; self-loops are ignored.
int stoer_wagner(vector<vector<int>> g) {
	int n = g.size(), best = INT_MAX;
	for (int ph = 1; ph < n; ph++) {
		vector<int> w = g[0];
		int s = 0, t = 0;
		for (int it = 0; it < n - ph; it++) {
			w[t] = INT_MIN, s = t, t = max_element(all(w)) - w.begin();
			for (int i = 0; i < n; i++) w[i] += g[t][i];
		}
		best = min(best, w[t] - g[t][t]);
		for (int i = 0; i < n; i++) g[s][i] += g[t][i];
		for (int i = 0; i < n; i++) g[i][s] = g[s][i];
		g[0][t] = INT_MIN; // t merged into s
	}
	return best;
}
