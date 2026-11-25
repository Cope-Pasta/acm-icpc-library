// String toolkit, all O(n). p[i] = length of the longest proper border of s[0..i]
vector<int> kmp(const string &s) {
	vector<int> p(s.size());
	for (int i = 1, j = 0; i < s.size(); i++) {
		while (j && s[i] != s[j]) j = p[j - 1];
		p[i] = j += s[i] == s[j];
	}
	return p;
}
// z[i] = lcp(s[i..], s) for i > 0, z[0] = 0
vector<int> Z(const string &s) {
	int n = s.size();
	vector<int> z(n);
	for (int i = 1, l = 0, r = 0; i < n; i++) {
		if (i < r) z[i] = min(r - i, z[i - l]);
		while (i + z[i] < n && s[i + z[i]] == s[z[i]]) z[i]++;
		if (i + z[i] > r) l = i, r = i + z[i];
	}
	return z;
}
// nx[i] = lcp(t[i..], t), ex[i] = lcp(s[i..], t)
void exkmp(const string &s, const string &t, vector<int> &nx, vector<int> &ex) {
	int m = t.size();
	auto z = Z(t + s);
	nx.resize(m), ex.resize(s.size());
	for (int i = 0; i < m; i++) nx[i] = i ? min(z[i], m - i) : m;
	for (int i = 0; i < s.size(); i++) ex[i] = min(z[m + i], m);
}
// start of the lexicographically smallest rotation (smallest such index)
int minRotation(string s) {
	int n = s.size(), i = 0, j = 1, k = 0;
	s += s;
	while (i < n && j < n && k < n)
		if (s[i + k] == s[j + k]) k++;
		else {
			(s[i + k] > s[j + k] ? i : j) += k + 1, k = 0;
			if (i == j) j++;
		}
	return min(i, j);
}
// l[i]: length of the longest palindrome centred at i / 2, i in [0, 2n); returns max l[i]
int manacher(const string &s, vector<int> &l) {
	int n = s.size() * 2;
	l.assign(n, 0);
	for (int i = 0, j = 0, k; i < n; i += k, j = max(j - k, 0)) {
		while (i >= j && i + j + 1 < n && s[(i - j) / 2] == s[(i + j + 1) / 2]) j++;
		l[i] = j;
		for (k = 1; i >= k && j >= k && l[i - k] != j - k; k++) l[i + k] = min(l[i - k], j - k);
	}
	return n ? *max_element(all(l)) : 0;
}
