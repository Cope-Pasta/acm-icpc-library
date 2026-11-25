vector<vector<int>> coef; // x^(k+j) = sum coef[j][i] * x^i

vector<int> mul(const vector<int> &a, const vector<int> &b) {
	int k = a.size();
	vector<int> ret(k + k - 1), c(k);
	for (int i = 0; i < k; ++i) {
		if (a[i] == 0) continue;
		for (int j = 0; j < k; ++j) ret[i + j] = (ret[i + j] + (ll)a[i] * b[j]) % mod;
	}
	for (int i = 0; i < k; ++i) c[i] = ret[i];
	for (int i = k; i < k + k - 1; ++i) {
		if (ret[i] == 0) continue;
		for (int j = 0; j < k; ++j) c[j] = (c[j] + (ll)ret[i] * coef[i - k][j]) % mod;
	}
	return c;
}

// h[n] = a[0] * h[n - 1] + ... + a[k - 1] * h[n - k], f = {h[0], ..., h[k - 1]}
int solve(const vector<int> &a, const vector<int> &f, ll n) {
	int k = a.size(), r = 0;
	coef.assign(k, vector<int>(k));
	for (int i = 0; i < k; ++i) coef[0][i] = a[k - 1 - i];
	for (int j = 1; j < k; ++j) {
		coef[j][0] = ((ll)coef[j - 1][k - 1] * coef[0][0]) % mod;
		for (int i = 1; i < k; ++i) {
			coef[j][i] = ((ll)coef[j - 1][k - 1] * coef[0][i] + coef[j - 1][i - 1]) % mod;
		}
	}
	vector<int> ret(k), x(k);
	ret[0] = 1;
	if (k > 1) x[1] = 1; else x[0] = coef[0][0];
	for (; n; n >>= 1, x = mul(x, x)) if (n & 1) ret = mul(ret, x);
	for (int i = 0; i < k; ++i) r = (r + (ll)ret[i] * f[i]) % mod;
	return r;
}
