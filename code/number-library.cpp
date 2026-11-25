int exgcd(int x, int y, int &a, int &b) { // extended gcd, ax + by = g.
	int a0 = 1, a1 = 0, b0 = 0, b1 = 1;
	while (y != 0) {
		a0 -= x / y * a1;
		swap(a0, a1);
		b0 -= x / y * b1;
		swap(b0, b1);
		x %= y;
		swap(x, y);
	}
	if (x < 0) a0 = -a0, b0 = -b0, x = -x;
	a = a0, b = b0;
	return x;
}

ll CRT(const vector<int> &p, const vector<int> &b) { // chinese remainder theorem
	ll N = accumulate(all(p), 1LL, multiplies<ll>()), ans = 0;
	int k = p.size();
	for (int i = 0; i < k; ++i) {
		ll mult = (inverse(N / p[i], p[i]) * (N / p[i])) % N;
		mult = (mult * b[i]) % N;
		ans += mult;
		ans %= N;
	}
	if (ans < 0) ans += N;
	return ans;
}

bool miller_rabin(ll n, ll b) { // miller-rabin prime test
	ll m = n - 1, cnt = 0;
	while (m % 2 == 0) m >>= 1, ++cnt;
	ll ret = fpow(b, m, n);
	if (ret == 1 || ret == n - 1) return true;
	--cnt;
	while (cnt >= 0) {
		ret = mult(ret, ret, n);
		if (ret == n - 1) return true;
		--cnt;
	}
	return false;
}

bool prime_test(ll n) {
	if (n < 2) return false;
	if (n < 4) return true;
	if (n == 3215031751LL) return false;
	const int BASIC[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
	for (int i = 0; i < 12 && BASIC[i] < n; ++i) {
		if (!miller_rabin(n, BASIC[i])) return false;
	}
	return true;
}

ll pollard_rho(ll n, ll seed) { // pollard-rho divisors factorization
	ll x, y;
	x = y = rand() % (n - 1) + 1;
	ll head = 1, tail = 2;
	while (true) {
		x = mult(x, x, n);
		x = (x + seed) % n;
		if (x == y) return n;
		ll d = gcd(max(x - y, y - x), n);
		if (1 < d && d < n) return d;
		if (++head == tail) y = x, tail <<= 1;
	}
}

void factorize(ll n, vector<ll> &divisor) {
	if (n == 1) return;
	if (prime_test(n)) divisor.push_back(n);
	else {
		ll d = n;
		while (d >= n) d = pollard_rho(n, rand() % (n - 1) + 1);
		factorize(n / d, divisor);
		factorize(d, divisor);
	}
}

// primtive root, finding the number with order p-1
int primtive_root(int p) {
	vector<int> factor;
	int tmp = p - 1;
	for (int i = 2; i * i <= tmp; ++i) {
		if (tmp % i == 0) {
			factor.push_back(i);
			while (tmp % i == 0) tmp /= i;
		}
	}
	if (tmp != 1) factor.push_back(tmp);
	for (int root = 1;; ++root) {
		bool flag = true;
		for (int i = 0; i < factor.size(); ++i) {
			if (fpow(root, (p - 1) / factor[i], p) == 1) {
				flag = false;
				break;
			}
		}
		if (flag) return root;
	}
}
