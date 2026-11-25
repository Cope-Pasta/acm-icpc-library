// Number theory for 64-bit moduli (products via __int128).
ll mult(ll a, ll b, ll m) { return (__int128)a * b % m; }
ll fpow(ll a, ll e, ll m) {
	ll r = 1 % m;
	for (a %= m; e; e >>= 1, a = mult(a, a, m))
		if (e & 1) r = mult(r, a, m);
	return r;
}
// returns g = gcd(x, y) >= 0 and sets a, b with a*x + b*y = g
ll exgcd(ll x, ll y, ll &a, ll &b) {
	if (!y) return a = x < 0 ? -1 : 1, b = 0, abs(x);
	ll g = exgcd(y, x % y, b, a);
	return b -= x / y * a, g;
}
// x in [0, prod p) with x = b[i] mod p[i]; p pairwise coprime, product fits in ll
ll CRT(const vector<int> &p, const vector<int> &b) {
	ll N = 1, r = 0, x, y;
	for (ll q : p) N *= q;
	for (int i = 0; i < p.size(); i++) {
		exgcd(N / p[i], p[i], x, y);
		r = (r + (x % p[i] + p[i]) * b[i] % p[i] * (N / p[i])) % N;
	}
	return (r + N) % N;
}
// deterministic Miller-Rabin for all n < 2^64
bool isPrime(ll n) {
	if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
	ll s = __builtin_ctzll(n - 1), d = n >> s;
	for (ll a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
		ll p = fpow(a, d, n), i = s;
		while (p != 1 && p != n - 1 && a % n && i--) p = mult(p, p, n);
		if (p != n - 1 && i != s) return 0;
	}
	return 1;
}
// Pollard rho: some nontrivial divisor of composite n, ~O(n^(1/4))
ll pollard(ll n) {
	ll x = 0, y = 0, t = 30, prd = 2, i = 1, q;
	auto f = [&](ll x) { return mult(x, x, n) + i; };
	while (t++ % 40 || __gcd(prd, n) == 1) {
		if (x == y) x = ++i, y = f(x);
		if ((q = mult(prd, max(x, y) - min(x, y), n))) prd = q;
		x = f(x), y = f(f(y));
	}
	return __gcd(prd, n);
}
// appends the prime factors of n (with multiplicity, unsorted) to d
void factorize(ll n, vector<ll> &d) {
	if (n == 1) return;
	if (isPrime(n)) d.push_back(n);
	else {
		ll x = pollard(n);
		factorize(x, d), factorize(n / x, d);
	}
}
// smallest primitive root (element of order p - 1) of prime p
ll primitiveRoot(ll p) {
	vector<ll> f;
	factorize(p - 1, f);
	for (ll g = 1;; g++)
		if (all_of(all(f), [&](ll q) { return fpow(g, (p - 1) / q, p) != 1; }))
			return g;
}
