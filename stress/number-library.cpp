#include "check/prelude.h"
LL mult(LL a, LL b, LL m) { return (__int128)a * b % m; }
LL fpow(LL a, LL e, LL m) { LL r = 1 % m; for (a %= m; e; e >>= 1, a = mult(a, a, m)) if (e & 1) r = mult(r, a, m); return r; }
LL inverse(LL a, LL m) { LL x0 = 1, x1 = 0, b = m; a %= m; while (b) { LL q = a / b; swap(a -= q * b, b); swap(x0 -= q * x1, x1); } return (x0 % m + m) % m; }
#include "code/number-library.cpp"

bool slow_prime(LL n) { if (n < 2) return false; for (LL d = 2; d * d <= n; ++d) if (n % d == 0) return false; return true; }

int main() {
	unsigned seed = 5; mt19937_64 rng(seed); srand(seed);
	auto fail = [&](const char *what, LL v) { printf("seed %u: %s %lld\n", seed, what, v); return 1; };
	for (int it = 0; it < 100000; ++it) { // exgcd
		int x = (int)(rng() % 2001) - 1000, y = (int)(rng() % 2001) - 1000, a, b;
		int g = exgcd(x, y, a, b);
		if (g != __gcd(abs(x), abs(y)) || a * x + b * y != g) return fail("exgcd", x * 10000LL + y);
	}
	vector<int> pr; for (int i = 2; pr.size() < 15; ++i) if (slow_prime(i)) pr.push_back(i);
	for (int it = 0; it < 5000; ++it) { // CRT with pairwise coprime moduli
		shuffle(pr.begin(), pr.end(), rng); int k = rng() % 6 + 1;
		vector<int> p(pr.begin(), pr.begin() + k), b(k); LL N = 1;
		for (int i = 0; i < k; ++i) b[i] = rng() % p[i], N *= p[i];
		LL x = CRT(p, b);
		if (x < 0 || x >= N) return fail("CRT range", x);
		for (int i = 0; i < k; ++i) if (x % p[i] != b[i]) return fail("CRT", x);
	}
	vector<bool> sieve(200001, true); sieve[0] = sieve[1] = false;
	for (int i = 2; i <= 200000; ++i) if (sieve[i]) for (LL j = (LL)i * i; j <= 200000; j += i) sieve[j] = false;
	for (int i = 0; i <= 200000; ++i) if (prime_test(i) != sieve[i]) return fail("prime_test small", i);
	for (int it = 0; it < 3000; ++it) { LL n = rng() % 1000000000000LL; if (prime_test(n) != slow_prime(n)) return fail("prime_test", n); }
	for (LL n : {3215031751LL, 3825123056546413051LL, 1000000007LL * 998244353, (1LL << 61) - 1}) if (prime_test(n) != (n == (1LL << 61) - 1)) return fail("prime_test special", n);
	for (int it = 0; it < 300; ++it) { // factorize
		LL n = it < 100 ? rng() % 1000000 + 2 : it < 200 ? (LL)(rng() % 1000000 + 2) * (LL)(rng() % 1000000 + 2) : (LL)1000000007 * (LL)(rng() % 1000000000 + 2);
		vector<LL> d; factorize(n, d); LL prod = 1;
		for (LL x : d) { if (!prime_test(x)) return fail("factor not prime", x); prod *= x; }
		if (prod != n) return fail("factorize product", n);
	}
	for (int p = 3; p < 3000; ++p) if (sieve[p]) { // primitive root vs order check
		int g = primtive_root(p), ord = 1; LL x = g % p;
		while (x != 1) x = x * g % p, ++ord;
		if (ord != p - 1) return fail("primtive_root", p);
		for (int h = 1; h < g; ++h) { int o = 1; LL y = h; while (y != 1) y = y * h % p, ++o; if (o == p - 1) return fail("primtive_root not smallest", p); }
	}
	if (primtive_root(998244353) != 3) return fail("primtive_root 998244353", 0);
	puts("OK number-library");
}
