// FFT: multiply(a, b) = convolution of integer vectors in O(n log n).
// long double; exact while result coefficients stay below ~1e15.
typedef complex<long double> C;
void fft(vector<C> &a) {
	int n = a.size(), L = 31 - __builtin_clz(n);
	static vector<C> rt(2, 1);
	for (static int k = 2; k < n; k *= 2) {
		rt.resize(n);
		C x = polar(1.0L, acosl(-1) / k);
		for (int i = k; i < 2 * k; i++) rt[i] = i & 1 ? rt[i / 2] * x : rt[i / 2];
	}
	vector<int> rev(n);
	for (int i = 0; i < n; i++) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
	for (int i = 0; i < n; i++)
		if (i < rev[i]) swap(a[i], a[rev[i]]);
	for (int k = 1; k < n; k *= 2)
		for (int i = 0; i < n; i += 2 * k)
			for (int j = 0; j < k; j++) {
				C z = rt[j + k] * a[i + j + k];
				a[i + j + k] = a[i + j] - z, a[i + j] += z;
			}
}
vector<ll> multiply(const vector<int> &a, const vector<int> &b) {
	if (a.empty() || b.empty()) return {};
	vector<ll> res(a.size() + b.size() - 1);
	int n = 1 << (32 - __builtin_clz(res.size()));
	vector<C> in(n), out(n);
	copy(all(a), in.begin());
	for (int i = 0; i < b.size(); i++) in[i].imag(b[i]);
	fft(in);
	for (C &x : in) x *= x;
	for (int i = 0; i < n; i++) out[i] = in[-i & (n - 1)] - conj(in[i]);
	fft(out);
	for (int i = 0; i < res.size(); i++) res[i] = llround(imag(out[i]) / (4 * n));
	return res;
}
