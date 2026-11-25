#include "check/prelude.h"
#include "code/suffix-array.cpp"

int main() {
	mt19937 rng(3);
	for (int it = 0; it < 20000; ++it) {
		int len = rng() % 30, A = 1 + rng() % 4;
		vector<int> s(len);
		for (auto &x : s) x = (int)(rng() % A) - 1;
		SA S(s); auto &sa = S.sa, &r = S.r, &h = S.h;
		vector<int> bsa(len); iota(bsa.begin(), bsa.end(), 0);
		sort(bsa.begin(), bsa.end(), [&](int a, int b) { return lexicographical_compare(s.begin() + a, s.end(), s.begin() + b, s.end()); });
		auto lcp = [&](int a, int b) { int k = 0; while (a + k < len && b + k < len && s[a + k] == s[b + k]) ++k; return k; };
		bool ok = sa == bsa;
		for (int i = 0; ok && i < len; ++i) ok = r[bsa[i]] == i;
		for (int i = 1; ok && i < len; ++i) ok = h[i] == lcp(bsa[i - 1], bsa[i]);
		for (int i = 0; ok && i < len; ++i) for (int j = 0; ok && j < len; ++j) ok = S.lcp(i, j) == lcp(i, j);
		if (!ok) {
			printf("mismatch on s =");
			for (int x : s) printf(" %d", x);
			puts(""); return 1;
		}
	}
	puts("OK suffix-array");
}
