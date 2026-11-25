#include "check/prelude.h"
#include "code/cyclic-longest-common-subsequence.cpp"

int brute_lcs(const string &a, const string &b) {
	vector<vector<int>> d(a.size() + 1, vector<int>(b.size() + 1));
	for (size_t i = 1; i <= a.size(); ++i) for (size_t j = 1; j <= b.size(); ++j)
		d[i][j] = a[i - 1] == b[j - 1] ? d[i - 1][j - 1] + 1 : max(d[i - 1][j], d[i][j - 1]);
	return d[a.size()][b.size()];
}

int main() {
	mt19937 rng(4);
	for (int it = 0; it < 30000; ++it) {
		int m = rng() % 10, n = rng() % 10, A = 1 + rng() % 3;
		string a, b;
		for (int i = 0; i < m; ++i) a += 'a' + rng() % A;
		for (int i = 0; i < n; ++i) b += 'a' + rng() % A;
		int br = brute_lcs(a, b); // max over all rotations of a
		for (int i = 1; i < m; ++i) br = max(br, brute_lcs(a.substr(i) + a.substr(0, i), b));
		int got = clcs(a, b);
		if (got != br) { printf("mismatch: a=\"%s\" b=\"%s\" got %d want %d\n", a.c_str(), b.c_str(), got, br); return 1; }
	}
	puts("OK cyclic-longest-common-subsequence");
}
