#include "check/prelude.h"
#include "code/number-library.cpp"

bool slow_prime(ll n) { if (n < 2) return false; for (ll d = 2; d * d <= n; ++d) if (n % d == 0) return false; return true; }

int main() {
	unsigned seed = 5; mt19937_64 rng(seed); srand(seed);
	auto fail = [&](const char *what, ll v) { printf("seed %u: %s %lld\n", seed, what, v); return 1; };
	for (int it = 0; it < 100000; ++it) { // exgcd
		int x = (int)(rng() % 2001) - 1000, y = (int)(rng() % 2001) - 1000; ll a, b;
		int g = exgcd(x, y, a, b);
		if (g != __gcd(abs(x), abs(y)) || a * x + b * y != g) return fail("exgcd", x * 10000LL + y);
	}
	vector<int> pr; for (int i = 2; pr.size() < 15; ++i) if (slow_prime(i)) pr.push_back(i);
	for (int it = 0; it < 5000; ++it) { // CRT with pairwise coprime moduli
		shuffle(pr.begin(), pr.end(), rng); int k = rng() % 6 + 1;
		vector<int> p(pr.begin(), pr.begin() + k), b(k); ll N = 1;
		for (int i = 0; i < k; ++i) b[i] = rng() % p[i], N *= p[i];
		ll x = CRT(p, b);
		if (x < 0 || x >= N) return fail("CRT range", x);
		for (int i = 0; i < k; ++i) if (x % p[i] != b[i]) return fail("CRT", x);
	}
	vector<bool> sieve(200001, true); sieve[0] = sieve[1] = false;
	for (int i = 2; i <= 200000; ++i) if (sieve[i]) for (ll j = (ll)i * i; j <= 200000; j += i) sieve[j] = false;
	for (int i = 0; i <= 200000; ++i) if (isPrime(i) != sieve[i]) return fail("prime_test small", i);
	for (int it = 0; it < 3000; ++it) { ll n = rng() % 1000000000000LL; if (isPrime(n) != slow_prime(n)) return fail("prime_test", n); }
	for (ll n : {3215031751LL, 3825123056546413051LL, 1000000007LL * 998244353, (1LL << 61) - 1}) if (isPrime(n) != (n == (1LL << 61) - 1)) return fail("prime_test special", n);
	for (int it = 0; it < 300; ++it) { // factorize
		ll n = it < 100 ? rng() % 1000000 + 2 : it < 200 ? (ll)(rng() % 1000000 + 2) * (ll)(rng() % 1000000 + 2) : (ll)1000000007 * (ll)(rng() % 1000000000 + 2);
		vector<ll> d; factorize(n, d); ll prod = 1;
		for (ll x : d) { if (!isPrime(x)) return fail("factor not prime", x); prod *= x; }
		if (prod != n) return fail("factorize product", n);
	}
	for (int p = 3; p < 3000; ++p) if (sieve[p]) { // primitive root vs order check
		int g = primitiveRoot(p), ord = 1; ll x = g % p;
		while (x != 1) x = x * g % p, ++ord;
		if (ord != p - 1) return fail("primtive_root", p);
		for (int h = 1; h < g; ++h) { int o = 1; ll y = h; while (y != 1) y = y * h % p, ++o; if (o == p - 1) return fail("primtive_root not smallest", p); }
	}
	if (primitiveRoot(998244353) != 3) return fail("primtive_root 998244353", 0);
	puts("OK number-library");
}
