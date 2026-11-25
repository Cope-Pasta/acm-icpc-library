#include "check/prelude.h"
const ll MOD = 1e9 + 7;
struct Map { // value at v for v in {N / k}: [0, s] directly, larger v by N / v
	ll n, s; vector<ll> lo, hi;
	Map(ll n) : n(n), s(sqrtl(n)), lo(s + 2), hi(n / max(s, 1LL) + 2) {}
	ll &at(ll v) { return v <= s ? lo[v] : hi[n / v]; }
};
vector<ll> pr, upto, pre; Map cnt(1);
#include "code/prefix-multiplicative.cpp"

int main() {
	unsigned seed = 9; mt19937 rng(seed);
	const int L = 200000; vector<int> pi(L + 1), d(L + 1); vector<bool> comp(L + 1);
	for (int i = 2; i <= L; ++i) { pi[i] = pi[i - 1] + !comp[i]; if (!comp[i]) for (ll j = (ll)i * i; j <= L; j += i) comp[j] = true; }
	for (int i = 1; i <= L; ++i) for (int j = i; j <= L; j += i) ++d[j];
	for (int it = 0; it < 300; ++it) {
		ll n = it < 50 ? it + 1 : rng() % L + 1;
		vector<ll> a; for (ll k = 1; k <= n; k = n / (n / k) + 1) a.push_back(n / k); // n // 1, n // 2, ... distinct, descending
		Map res = Pi(n, a);
		for (ll v : a) if (res.at(v) != pi[v]) { printf("seed %u: Pi(%lld) at %lld want %d got %lld\n", seed, n, v, pi[v], res.at(v)); return 1; }
		// get: sum of d(m) for m <= n, d(p^c) = c + 1
		cnt = res; pr.clear();
		for (int p = 2; ; ++p) if (!comp[p]) { pr.push_back(p); if ((ll)p * p > n) break; } // last prime is a sentinel
		upto.assign(pr.back() + 1, 0); // upto[p] = pi(p)
		for (ll p = 1; p < (ll)upto.size(); ++p) upto[p] = pi[p];
		pre.assign(64, 0); for (int c = 0; c < 64; ++c) pre[c] = c + 1;
		ll want = 0; for (int m = 1; m <= n; ++m) want += d[m];
		if (get(n, 0) != want % MOD) { printf("seed %u: get(%lld) want %lld got %lld\n", seed, n, want, get(n, 0)); return 1; }
	}
	puts("OK prefix-multiplicative");
}
