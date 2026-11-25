// sa[i]: i-th smallest suffix, r[i]: rank of suffix i, h[i] = lcp(sa[i - 1], sa[i]); all 0-indexed
int n;
vector<int> sa, r, h;
vector<vector<int>> mv;
void construct(const vector<int /* Type */> &s) {
	n = s.size(), sa.assign(n, 0), r.assign(n + n, 0), h.assign(n, 0); // r is `doubled`
	vector<pair<int /* Type */, int>> ord(n);
	for (int i = 0; i < n; ++i) ord[i] = make_pair(s[i], i);
	sort(all(ord));
	for (int i = 0; i < n; ++i) {
		sa[i] = ord[i].second;
		r[sa[i]] = (i == 0 ? 1 : r[sa[i - 1]] + (ord[i - 1].first != ord[i].first));
	}
	vector<int> tr(n), tsa(n), c(n + 1);
	for (int l = 1; l < n; l <<= 1) {
		int cnt = 0;
		for (int i = 0; i < n; ++i)
			if (sa[i] + l >= n) tsa[cnt++] = sa[i];
		for (int i = 0; i < n; ++i)
			if (sa[i] >= l) tsa[cnt++] = sa[i] - l;
		fill(all(c), 0);
		for (int i = 0; i < n; ++i) ++c[r[i]];
		for (int i = 1; i <= n; ++i) c[i] += c[i - 1];
		for (int i = n - 1; i >= 0; --i) sa[--c[r[tsa[i]]]] = tsa[i];
		tr[sa[0]] = 1;
		for (int i = 1; i < n; ++i) {
			tr[sa[i]] = tr[sa[i - 1]] + (r[sa[i]] != r[sa[i - 1]] || r[sa[i] + l] != r[sa[i - 1] + l]);
		}
		for (int i = 0; i < n; ++i) r[i] = tr[i];
		if (r[sa[n - 1]] == n) break;
	}
	r.resize(n);
	for (int &x : r) --x;
	for (int i = 0, k = 0; i < n; ++i) { /* Height && RMQ */
		if (k > 0) --k;
		if (r[i] == 0) {
			k = 0;
			continue;
		}
		for (int j = sa[r[i] - 1]; max(i, j) + k < n && s[i + k] == s[j + k]; ++k);
		h[r[i]] = k;
	}
	mv.assign(__lg(max(n, 1)) + 1, h);
	for (int k = 1; k < (int)mv.size(); ++k) {
		for (int i = 0; i + (1 << k) <= n; ++i) mv[k][i] = min(mv[k - 1][i], mv[k - 1][i + (1 << (k - 1))]);
	}
}

int askRMQ(int l, int r) { // min of h[l...r]
	int k = __lg(r - l + 1);
	return min(mv[k][l], mv[k][r - (1 << k) + 1]);
}

int LCP(int i, int j) { // suffixes i and j
	if (i == j) return n - i;
	i = r[i], j = r[j];
	if (i > j) swap(i, j);
	return askRMQ(i + 1, j);
}
