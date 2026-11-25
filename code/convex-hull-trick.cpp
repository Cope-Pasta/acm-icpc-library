// Convex hull trick, O(log n): add lines k*x+b with strictly increasing k;
// get(x) = min over lines. For max: add (-k, -b), answer -get(x).
// db * dk products must fit in ll.
struct CHT {
	vector<ll> K, B;
	ll f(int i, ll x) { return K[i] * x + B[i]; }
	void add(ll k, ll b) {
		for (int n; (n = K.size()) > 1 &&
		    (B[n - 2] - B[n - 1]) * (k - K[n - 1]) <=
		        (B[n - 1] - b) * (K[n - 1] - K[n - 2]);)
			K.pop_back(), B.pop_back();
		K.push_back(k), B.push_back(b);
	}
	ll get(ll x) {
		int l = 0, r = K.size() - 1;
		while (l < r) {
			int m = (l + r) / 2;
			f(m, x) <= f(m + 1, x) ? r = m : l = m + 1;
		}
		return f(l, x);
	}
};
