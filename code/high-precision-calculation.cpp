const int B = 10000, D = 4;
struct bnum_t { // b[0] is the lowest digit (base B)
	vector<int> b;
	bnum_t(long long n = 0) { do b.push_back(n % B), n /= B; while (n); }
	bnum_t(const string &str) {
		for (int i = (int)str.size() - 1; i >= 0; i -= D) {
			int v = 0;
			for (int j = max(0, i - D + 1); j <= i; ++j) v = v * 10 + str[j] - '0';
			b.push_back(v);
		}
		trim();
	}
	void trim() { while (b.size() > 1 && b.back() == 0) b.pop_back(); }
	friend ostream &operator<<(ostream &os, const bnum_t &r) {
		os << (r.b.empty() ? 0 : r.b.back());
		for (int i = (int)r.b.size() - 2; i >= 0; --i) os << setfill('0') << setw(D) << r.b[i];
		return os;
	}
	bnum_t operator+(const bnum_t &r) const {
		bnum_t f; f.b.assign(max(b.size(), r.b.size()) + 1, 0);
		for (int i = 0; i + 1 < (int)f.b.size(); ++i) {
			f.b[i] += (i < (int)b.size() ? b[i] : 0) + (i < (int)r.b.size() ? r.b[i] : 0);
			f.b[i + 1] = f.b[i] / B, f.b[i] %= B;
		}
		f.trim();
		return f;
	}
	bnum_t operator-(const bnum_t &r) const { // assume a >= b
		bnum_t f = *this;
		for (int i = 0; i < (int)f.b.size(); ++i) {
			f.b[i] -= i < (int)r.b.size() ? r.b[i] : 0;
			if (f.b[i] < 0) f.b[i] += B, --f.b[i + 1];
		}
		f.trim();
		return f;
	}
	bnum_t operator*(const bnum_t &r) const {
		bnum_t f; f.b.assign(b.size() + r.b.size(), 0);
		for (int i = 0; i < (int)b.size(); ++i) {
			long long c = 0;
			for (int j = 0; j < (int)r.b.size() || c; ++j) {
				c += f.b[i + j] + (j < (int)r.b.size() ? 1LL * b[i] * r.b[j] : 0);
				f.b[i + j] = c % B, c /= B;
			}
		}
		f.trim();
		return f;
	}
	bnum_t operator/(int r) const {
		bnum_t f = *this; long long rem = 0;
		for (int i = (int)f.b.size() - 1; i >= 0; --i) rem = rem * B + f.b[i], f.b[i] = rem / r, rem %= r;
		f.trim();
		return f;
	}
};
