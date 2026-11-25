// Hamiltonian cycles on an n x m grid mod `mod`, huge n via transfer matrix
// (matrix_t with * and power). m <= 7 or so. Plug states: m + 1 base-3 digits
// (0 none, 1 "(", 2 ")"), plus -1 = cycle closed on the last cell.
int plugDP(ll n, int m) {
	vector<int> p3(m + 2, 1);
	for (int i = 1; i <= m + 1; ++i) p3[i] = p3[i - 1] * 3;
	auto get = [&](int s, int j) { return s / p3[j] % 3; };
	auto put = [&](int s, int j, int v) { return s + (v - get(s, j)) * p3[j]; };
	auto mate = [&](int s, int j) { // position of the bracket matching the one at j
		for (int d = get(s, j) == 1 ? 1 : -1, c = 0;; j += d)
			if (int x = get(s, j); x && !(c += x == 1 ? d : -d)) return j;
	};
	auto row = [&](int s) { // fill one row starting from plug state s
		map<int, int> f{{s, 1}};
		for (int j = 0; j < m; ++j) {
			map<int, int> h;
			for (auto [k, v] : f) {
				auto add = [&](int s) { h[s] = (h[s] + v) % mod; };
				int x = get(k, j), y = get(k, j + 1),
				    t = put(put(k, j, 0), j + 1, 0);
				if (!x && !y) add(put(put(t, j, 1), j + 1, 2));
				else if (!x || !y) add(k), add(put(put(t, j, y), j + 1, x));
				else if (x == 1 && y == 2) {
					if (j == m - 1 && !t)
						add(-1); // close the only loop on the last cell of a row
				} else if (x == 2 && y == 1) add(t);
				else if (x == 1) add(put(t, mate(k, j + 1), 1));
				else add(put(t, mate(k, j), 2));
			}
			f = h;
		}
		map<int, int> r;
		for (auto [k, v] : f)
			if (k < 0 || !get(k, m)) r[k < 0 ? k : k * 3] = v;
		return r;
	};
	vector<int> st{0, -1};
	map<int, int> id{{0, 0}, {-1, 1}};
	vector<map<int, int>> R;
	for (int i = 0; i < (int)st.size(); ++i) {
		R.push_back(st[i] < 0 ? map<int, int>() : row(st[i]));
		for (auto [k, v] : R[i])
			if (!id.count(k)) id[k] = st.size(), st.push_back(k);
	}
	matrix_t<int> a(st.size());
	for (int i = 0; i < (int)st.size(); ++i)
		for (auto [k, v] : R[i]) a.x[id[k]][i] = v;
	return a.power(n).x[1][0];
}
