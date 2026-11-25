// Non-negative big integer, base 1e9 limbs (b[0] lowest): +, - (needs a >= b), *, / by ll (< 9e9), <<.
const int B = 1e9;
struct Big {
	vector<ll> b;
	Big(ll n = 0) {
		do b.push_back(n % B), n /= B;
		while (n);
	}
	Big(const string &s) {
		for (int i = s.size(), j; i > 0; i = j) j = max(0, i - 9), b.push_back(stoll(s.substr(j, i - j)));
		trim();
	}
	Big &trim() {
		while (b.size() > 1 && !b.back()) b.pop_back();
		return *this;
	}
	friend ostream &operator<<(ostream &o, const Big &r) {
		o << r.b.back();
		for (int i = r.b.size() - 1; i--;) o << setfill('0') << setw(9) << r.b[i];
		return o;
	}
	Big add(const Big &r, int s) const {
		Big f = *this;
		f.b.resize(max(b.size(), r.b.size()) + 1);
		for (int i = 0; i + 1 < f.b.size(); i++) {
			f.b[i] += s * (i < r.b.size() ? r.b[i] : 0);
			ll c = f.b[i] < 0 ? -1 : f.b[i] >= B;
			f.b[i] -= c * B, f.b[i + 1] += c;
		}
		return f.trim();
	}
	Big operator+(const Big &r) const { return add(r, 1); }
	Big operator-(const Big &r) const { return add(r, -1); }
	Big operator*(const Big &r) const {
		Big f;
		f.b.assign(b.size() + r.b.size(), 0);
		for (int i = 0; i < b.size(); i++)
			for (ll j = 0, c = 0; j < r.b.size() || c; j++) c += f.b[i + j] + (j < r.b.size() ? b[i] * r.b[j] : 0), f.b[i + j] = c % B, c /= B;
		return f.trim();
	}
	Big operator/(ll r) const {
		Big f = *this;
		ll m = 0;
		for (int i = b.size(); i--;) m = m * B + b[i], f.b[i] = m / r, m %= r;
		return f.trim();
	}
};
