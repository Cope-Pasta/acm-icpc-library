// Karp minimum mean cycle (negative weights ok), O(nm). Vertices 0..n-1.
// maxdbl if there is no cycle.
struct edge_t {
	int u, v, w;
};
double mmc(int n, const vector<edge_t> &e) {
	vector<vector<int>> d(n + 1, vector<int>(n, maxint));
	d[0].assign(n, 0);
	for (int i = 0; i < n; i++)
		for (auto [u, v, w] : e)
			if (d[i][u] != maxint) d[i + 1][v] = min(d[i + 1][v], d[i][u] + w);
	double res = maxdbl;
	for (int v = 0; v < n; v++) {
		double x = -maxdbl;
		for (int k = 0; k < n; k++)
			x = max(x, (double(d[n][v]) - d[k][v]) / (n - k));
		if (d[n][v] != maxint) res = min(res, x);
	}
	return res;
}
