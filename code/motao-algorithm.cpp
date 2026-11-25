// Mo's algorithm: offline range queries Q[i] = [l, r] (0-indexed, inclusive) on positions 0..n-1 with O(n sqrt q)
// calls add(i, d): d = 1 inserts position i, d = -1 removes it. Returns ans[i] = get() on range Q[i].
template <class A, class G>
vector<ll> mo(int n, const vector<pair<int, int>> &Q, A add, G get) {
	int q = Q.size(), B = max(1, int(n / sqrt(q + 1))), L = 0, R = -1;
	vector<int> s(q);
	vector<ll> ans(q);
	iota(all(s), 0);
	auto key = [&](int i) { auto [l, r] = Q[i]; return pair(l / B, l / B & 1 ? -r : r); };
	sort(all(s), [&](int i, int j) { return key(i) < key(j); });
	for (int i : s) {
		auto [l, r] = Q[i];
		while (R < r) add(++R, 1);
		while (L > l) add(--L, 1);
		while (R > r) add(R--, -1);
		while (L < l) add(L++, -1);
		ans[i] = get();
	}
	return ans;
}
