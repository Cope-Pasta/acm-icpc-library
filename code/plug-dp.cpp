// Count Hamiltonian cycles on n * m grid, plugs 0...m, m <= maxm
const int maxm = 7, pow3[] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561};
int cnt; vector<int> valid, ID; vector<vector<int>> match;
int gb3(int v, int bit) { return (v / pow3[bit]) % 3; }
int mb3(int v, int bit, int value) { return v - pow3[bit] * gb3(v, bit) + pow3[bit] * value; }
int ub3(int v, int bit) { return v - pow3[bit] * gb3(v, bit); }
void upd(int &x, int v) { x += v; x %= mod; }

void dfs(int p, int st, int lt, int m) {
	if (p > m) {
		if (lt == 0) {
			valid[st] = true;
			if (st % 3 == 0) ID[st] = cnt++; // omitted
			vector<int> sk;
			for (int j = 0; j <= m; ++j) {
				if (gb3(st, j) == 1) {
					sk.push_back(j);
				} else if (gb3(st, j) == 2) {
					match[st][j] = sk.back();
					match[st][sk.back()] = j, sk.pop_back();
				}
			}
		}
	} else {
		dfs(p + 1, st, lt, m); // #
		dfs(p + 1, mb3(st, p, 1), lt + 1, m); // (
		if (lt) dfs(p + 1, mb3(st, p, 2), lt - 1, m); // )
	}
}

int plugDP(int n, int m) {
	int S = pow3[m + 1]; cnt = 0;
	valid.assign(S, 0), ID.assign(S, -1), match.assign(S, vector<int>(m + 1));
	dfs(0, 0, 0, m);
	matrix_t<int> mat(cnt), mat2(cnt);
	for (int start = 0; start < S; ++start) {
		if (ID[start] == -1) continue;
		for (int fi = 0; fi < 2; ++fi) { // Last row flag
			vector<vector<int>> f(m + 1, vector<int>(S)); vector<int> g(S);
			f[0][start] = 1;
			for (int j = 0; j <= m; ++j) {
				for (int k = 0; k < S; ++k) {
					if (!valid[k]) continue;
					if (j == m) {
						if (!fi && !gb3(k, j)) { // i != n
							upd(g[ub3(k, j) * 3], f[j][k]); // Unmark last bit
						}
					} else {
						 // Consider mp[i][j] valid ? (bit1 = bit2 = 0)
						int bit1 = gb3(k, j), bit2 = gb3(k, j + 1),
							t = ub3(ub3(k, j), j + 1), tt;
						if (bit1 == 1 && bit2 == 2) { // Merge two brackets
							if (fi && j + 1 == m) { // i == n
								upd(f[j + 1][t], f[j][k]);
							}
						} else if (bit1 == 2 && bit2 == 1) {
							upd(f[j + 1][t], f[j][k]);
						} else if (!bit1 && !bit2) { // bit1 == 0 && bit2 == 0
							tt = mb3(mb3(t, j, 1), j + 1, 2);
							upd(f[j + 1][tt], f[j][k]);
						} else if (bit1 == bit2) {
							if (bit1 == 1) { // bit1 == 1 && bit2 == 1
								tt = mb3(t, match[k][j + 1], 1);
								upd(f[j + 1][tt], f[j][k]);
							} else { // bit1 == 2 && bit2 == 2
								tt = mb3(t, match[k][j], 2);
								upd(f[j + 1][tt], f[j][k]);
							}
						} else { // bit1 == 0 || bit2 == 0
							upd(f[j + 1][k], f[j][k]);
							swap(bit1, bit2);
							tt = mb3(mb3(t, j, bit1), j + 1, bit2);
							upd(f[j + 1][tt], f[j][k]);
						}
					}
				}
			}
			for (int i = 0; i < S; ++i) if (ID[i] != -1) {
				if (fi) mat2.x[ID[i]][ID[start]] = f[m][i];
				else mat.x[ID[i]][ID[start]] = g[i];
			}
		}
	}
	mat = mat2 * mat.power(n - 1);
	return mat.x[ID[0]][ID[0]];
}
