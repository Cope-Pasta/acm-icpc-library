#include "check/prelude.h"
const int mod = 1e9 + 7;
template<typename T> struct matrix_t { // code/matrix.cpp interface plus * and power
	int n; vector<vector<T>> x;
	matrix_t(int n, T v = 0) : n(n), x(n, vector<T>(n)) { for (int i = 0; i < n; ++i) x[i][i] = v; }
	matrix_t operator*(const matrix_t &o) const {
		matrix_t r(n);
		for (int i = 0; i < n; ++i) for (int k = 0; k < n; ++k) for (int j = 0; j < n; ++j) r.x[i][j] = (r.x[i][j] + 1LL * x[i][k] * o.x[k][j]) % mod;
		return r;
	}
	matrix_t power(LL e) const { matrix_t r(n, 1), b = *this; for (; e; e >>= 1, b = b * b) if (e & 1) r = r * b; return r; }
};
#include "code/plug-dp.cpp"

int R, W; LL cycles; vector<int> vis;
void go(int v, int d) { // directed Hamiltonian cycles through cell 0
	static const int dr[] = {0, 0, 1, -1}, dc[] = {1, -1, 0, 0};
	for (int k = 0; k < 4; ++k) {
		int nr = v / W + dr[k], nc = v % W + dc[k];
		if (nr < 0 || nc < 0 || nr >= R || nc >= W) continue;
		int u = nr * W + nc;
		if (u == 0 && d == R * W) ++cycles;
		else if (!vis[u]) vis[u] = 1, go(u, d + 1), vis[u] = 0;
	}
}

int main() { // number of Hamiltonian cycles on an R x W grid
	for (R = 2; R <= 6; ++R) for (W = 1; W <= 7 && R * W <= 30; ++W) {
		vis.assign(R * W, 0), vis[0] = 1, cycles = 0, go(0, 1);
		int got = plugDP(R, W);
		if (got != cycles / 2) { printf("mismatch on %dx%d: got %d want %lld\n", R, W, got, cycles / 2); return 1; }
	}
	puts("OK plug-dp");
}
