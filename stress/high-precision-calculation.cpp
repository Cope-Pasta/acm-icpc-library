#include "check/prelude.h"
#include "code/high-precision-calculation.cpp"

typedef __int128 i128;
string str(i128 v) { string s; do s += char('0' + v % 10), v /= 10; while (v); return string(s.rbegin(), s.rend()); }
string str(const Big &v) { stringstream ss; ss << v; return ss.str(); }

int main() {
	mt19937_64 rng(7);
	auto rnd = [&]() -> ll { // mix of tiny, digit-boundary and large values
		int t = rng() % 4;
		if (t == 0) return rng() % 20;
		if (t == 1) { ll p = 1; for (int k = rng() % 18; k; --k) p *= 10; return max(0LL, p - (ll)(rng() % 3)); }
		return rng() % (ll)1e18;
	};
	for (int it = 0; it < 200000; ++it) {
		ll a = rnd(), b = rnd(); int k = 1 + rng() % 1000000;
		if (a < b) swap(a, b);
		Big x(a), y(to_string(b));
		vector<pair<string, string>> chk = {
			{str(x), str((i128)a)}, {str(y), str((i128)b)}, {str(x + y), str((i128)a + b)}, {str(x - y), str((i128)a - b)},
			{str(x * y), str((i128)a * b)}, {str(x / k), str((i128)a / k)}, {str(x * Big(k)), str((i128)a * k)}};
		for (auto &[got, want] : chk) if (got != want) {
			printf("mismatch: a=%lld b=%lld k=%d got %s want %s\n", a, b, k, got.c_str(), want.c_str());
			return 1;
		}
	}
	puts("OK high-precision-calculation");
}
