#include "check/prelude.h"
#include "code/matrix.cpp"
// determinant() is Euclid-style elimination with an exact zero test: meant for integer T
// (small entries, intermediate values grow) or a modular field type, not for double.

const int P = 1e9 + 7;
ll pw(ll a, ll e) { ll r = 1; for (a %= P; e; e >>= 1, a = a * a % P) if (e & 1) r = r * a % P; return r; }
struct mint {
	ll v;
	mint(ll x = 0) : v((x % P + P) % P) {}
	mint operator-() const { return mint(-v); }
	mint operator*(mint o) const { return mint(v * o.v); }
	mint operator/(mint o) const { return mint(v * pw(o.v, P - 2)); }
	mint &operator-=(mint o) { v = (v - o.v + P) % P; return *this; }
	bool operator!=(int x) const { return v != x; }
};

ll brute(const vector<vector<ll>> &a, ll mod = 0) { // permutation expansion
	int n = a.size(); vector<int> p(n); iota(p.begin(), p.end(), 0); ll s = 0;
	do {
		ll t = 1; for (int i = 0; i < n; ++i) t = mod ? t * a[i][p[i]] % mod : t * a[i][p[i]];
		int inv = 0; for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) inv += p[i] > p[j];
		s += inv & 1 ? -t : t; if (mod) s = (s % mod + mod) % mod;
	} while (next_permutation(p.begin(), p.end()));
	return s;
}

int main() {
	unsigned seed = 1; mt19937 rng(seed);
	for (int it = 0; it < 5000; ++it) {
		int n = rng() % 5 + 1;
		matrix_t<ll> m(n); matrix_t<mint> q(n); vector<vector<ll>> a(n, vector<ll>(n)), b = a;
		for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
			a[i][j] = m.x[i][j] = (int)(rng() % 7) - 3;
			b[i][j] = rng() % P; q.x[i][j] = b[i][j];
		}
		ll want = brute(a), got = m.determinant(), wq = brute(b, P), gq = q.determinant().v;
		if (got != want || gq != wq) { printf("seed %u it %d n %d: want %lld got %lld, mod want %lld got %lld\n", seed, it, n, want, got, wq, gq); return 1; }
	}
	matrix_t<ll> id(4, 1); if (id.determinant() != 1) { puts("identity"); return 1; }
	puts("OK matrix");
}
