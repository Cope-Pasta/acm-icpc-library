// n-th term (0-indexed) of h[i] = tr[0]*h[i-1] + ... + tr[k-1]*h[i-k],
// given h[0..k-1] = S. O(k^2 log n): x^n mod the characteristic polynomial.
// Needs global mod.
typedef vector<ll> Poly;
ll linRec(Poly tr, Poly S, ll n) {
	int k = tr.size();
	auto mul = [&](Poly a, Poly b) {
		Poly r(2 * k + 1);
		for (int i = 0; i <= k; i++)
			for (int j = 0; j <= k; j++) r[i + j] = (r[i + j] + a[i] * b[j]) % mod;
		for (int i = 2 * k; i > k; i--)
			for (int j = 0; j < k; j++)
				r[i - 1 - j] = (r[i - 1 - j] + r[i] * tr[j]) % mod;
		return r.resize(k + 1), r;
	};
	Poly p(k + 1), e(p);
	p[0] = e[1] = 1;
	for (++n; n; n /= 2, e = mul(e, e))
		if (n & 1) p = mul(p, e);
	ll r = 0;
	for (int i = 0; i < k; i++) r = (r + p[i + 1] * S[i]) % mod;
	return r;
}
