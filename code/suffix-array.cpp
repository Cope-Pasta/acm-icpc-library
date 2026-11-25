// Suffix array of s (any ints), O(n log n). sa[i]: i-th smallest suffix,
// r[i]: rank of suffix i, h[i] = lcp(sa[i - 1], sa[i]), h[0] = 0.
// lcp(i, j): lcp of suffixes i and j in O(1) (sparse table).
struct SA {
	int n;
	vector<int> sa, r, h;
	vector<vector<int>> t;
	SA(const vector<int> &s) : n(s.size()) {
		int N = n + 1, k = 0, a, b, lim = N;
		vector<int> c(all(s)), x(N), y(N), ws(N);
		sort(all(c));
		for (int i = 0; i < n; i++)
			x[i] = lower_bound(all(c), s[i]) - c.begin() + 1;
		c = x, sa = h = y, iota(all(sa), 0);
		for (int j = 0, p = 0; p < N; j = max(1, j * 2), lim = p) {
			p = j, iota(all(y), N - j);
			for (int i = 0; i < N; i++)
				if (sa[i] >= j) y[p++] = sa[i] - j;
			fill(all(ws), 0);
			for (int i = 0; i < N; i++) ws[x[i]]++;
			for (int i = 1; i < lim; i++) ws[i] += ws[i - 1];
			for (int i = N; i--;) sa[--ws[x[y[i]]]] = y[i];
			swap(x, y), p = 1, x[sa[0]] = 0;
			for (int i = 1; i < N; i++)
				a = sa[i - 1], b = sa[i],
				x[b] = y[a] == y[b] && y[a + j] == y[b + j] ? p - 1 : p++;
		}
		for (int i = 0, j; i < n; h[x[i++]] = k)
			for (k -= k > 0, j = sa[x[i] - 1]; c[i + k] == c[j + k]; k++);
		sa.erase(sa.begin()), h.erase(h.begin()), x.pop_back(), r = x;
		for (int &v : r) v--;
		t.assign(__lg(N) + 1, h);
		for (int k = 1; k < t.size(); k++)
			for (int i = 0; i + (1 << k) <= n; i++)
				t[k][i] = min(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
	}
	int lcp(int i, int j) {
		if (i == j) return n - i;
		tie(i, j) = minmax(r[i], r[j]);
		int k = __lg(j - i);
		return min(t[k][i + 1], t[k][j - (1 << k) + 1]);
	}
};
