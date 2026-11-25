// Pi: prime counts pi(v) for every v in a = {n/1, n/2, ..., 1} (distinct, descending), O(n^(3/4)).
// Map r(n): r.at(v) is lo[v] for v <= s = sqrt(n), else hi[n / v].
Map Pi(ll n, const vector<ll> &a) {
	Map r(n);
	for (ll v : a) r.at(v) = v - 1;
	for (ll i = 2; i <= r.s; i++)
		if (r.at(i) != r.at(i - 1)) {
			ll f = r.at(i - 1);
			for (ll v : a)
				if (v < i * i) break;
				else r.at(v) -= r.at(v / i) - f;
		}
	return r;
}
// get(n, 0) = sum of multiplicative g(m), m <= n, where g(p^c) = pre[c] for every prime p.
// pr: primes <= sqrt(n) plus one larger sentinel, upto[p] = pi(p), cnt = Pi(n, a).
ll get(ll n, int i) {
	ll p = pr[i];
	if (p > n) return 1;
	if (p * p > n) return (1 + (cnt.at(n) - upto[p] + 1) * pre[1]) % MOD;
	__int128 r = 0;
	for (ll f = 1, c = 0; f <= n; f *= p, c++) r += pre[c] * get(n / f, i + 1);
	return r % MOD;
}
