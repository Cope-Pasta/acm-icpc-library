// k-d tree: k nearest neighbours by squared distance in D dims. Build O(n log n), query ~O(log n) on random data.
// KD t(pts) (pts non-empty, reordered into t.p); t.knn(q, k): max-heap of (dist2, index into t.p) of the k nearest.
typedef vector<ll> P;
struct KD {
	vector<P> p;
	int D, k;
	P q;
	priority_queue<pair<ll, int>> pq;
	KD(vector<P> p) : p(p), D(p[0].size()) { build(0, p.size(), 0); }
	void build(int l, int r, int d) {
		if (r - l < 2) return;
		int m = (l + r) / 2;
		nth_element(p.begin() + l, p.begin() + m, p.begin() + r, [&](P &a, P &b) { return a[d] < b[d]; });
		build(l, m, (d + 1) % D), build(m + 1, r, (d + 1) % D);
	}
	void go(int l, int r, int d) {
		if (l >= r) return;
		int m = (l + r) / 2, a = l, b = m, c = m + 1, e = r;
		ll s = 0, t = q[d] - p[m][d];
		for (int i = 0; i < D; ++i) s += (q[i] - p[m][i]) * (q[i] - p[m][i]);
		pq.push({s, m});
		if ((int)pq.size() > k) pq.pop();
		if (t >= 0) swap(a, c), swap(b, e);
		go(a, b, (d + 1) % D);
		if ((int)pq.size() < k || t * t <= pq.top().first) go(c, e, (d + 1) % D);
	}
	priority_queue<pair<ll, int>> knn(P x, int K) { return q = x, k = K, pq = {}, go(0, p.size(), 0), pq; }
};
