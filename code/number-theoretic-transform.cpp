// NTT mod 998244353 = 119*2^23+1 (primitive root 3), in place, O(n log n).
// a.size() must be a power of 2 (<= 2^23).
// inv = true computes the inverse (includes 1/n).
const int mod = 998244353;
ll pw(ll b, ll e) {
	ll r = 1;
	for (; e; e >>= 1, b = b * b % mod)
		if (e & 1) r = r * b % mod;
	return r;
}
void ntt(vector<int> &a, bool inv) {
	int n = a.size();
	for (int i = 1, j = 0; i < n; i++) {
		int bit = n >> 1;
		for (; j & bit; bit >>= 1) j ^= bit;
		if (i < (j ^= bit)) swap(a[i], a[j]);
	}
	for (int k = 1; k < n; k *= 2) {
		ll w = pw(3, (mod - 1) / 2 / k * (inv ? 2 * k - 1 : 1));
		for (int i = 0; i < n; i += 2 * k) {
			ll x = 1;
			for (int j = i; j < i + k; j++, x = x * w % mod) {
				int u = a[j], v = a[j + k] * x % mod;
				a[j] = (u + v) % mod, a[j + k] = (u - v + mod) % mod;
			}
		}
	}
	if (inv) {
		ll ni = pw(n, mod - 2);
		for (int &x : a) x = x * ni % mod;
	}
}
