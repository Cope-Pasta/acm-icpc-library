#include "check/prelude.h"
#include "code/string-library.cpp"

int main() {
	mt19937 rng(1);
	for (int it = 0; it < 100000; ++it) {
		int n = rng() % 12, m = rng() % 12, A = 1 + rng() % 3;
		string s, t;
		for (int i = 0; i < n; ++i) s += 'a' + rng() % A;
		for (int i = 0; i < m; ++i) t += 'a' + rng() % A;
		auto fail = [&](const char *what) { printf("%s mismatch: s=\"%s\" t=\"%s\"\n", what, s.c_str(), t.c_str()); return 1; };
		auto k = kmp(s); // next[i]: longest proper border of s[0..i]
		for (int i = 0; i < n; ++i) {
			int b = 0;
			for (int L = 1; L <= i; ++L) if (s.compare(0, L, s, i + 1 - L, L) == 0) b = L;
			if (k[i] != b) return fail("kmp");
		}
		vector<int> nx, ex; exkmp(s, t, nx, ex); // ex[i]: lcp(s[i..], t), next[i]: lcp(t[i..], t)
		for (int i = 0; i < n; ++i) { int l = 0; while (i + l < n && l < m && s[i + l] == t[l]) ++l; if (ex[i] != l) return fail("exkmp ex"); }
		for (int i = 0; i < m; ++i) { int l = 0; while (i + l < m && t[i + l] == t[l]) ++l; if (nx[i] != l) return fail("exkmp next"); }
		auto z = Z(s);
		for (int i = 1; i < n; ++i) { int l = 0; while (i + l < n && s[i + l] == s[l]) ++l; if (z[i] != l) return fail("z_function"); }
		vector<int> l; int best = manacher(s, l), bb = 0;
		for (int i = 0; i < n; ++i) for (int j = i; j < n; ++j) {
			string u = s.substr(i, j - i + 1);
			if (u == string(u.rbegin(), u.rend())) bb = max(bb, j - i + 1);
		}
		if (best != bb) return fail("manacher");
		if (n) {
			int mr = minRotation(s), br = 0;
			for (int i = 1; i < n; ++i) if (s.substr(i) + s.substr(0, i) < s.substr(br) + s.substr(0, br)) br = i;
			if (mr != br) return fail("minimum_representation");
		}
	}
	puts("OK string-library");
}
