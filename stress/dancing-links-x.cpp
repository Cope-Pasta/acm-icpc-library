#include "check/prelude.h"
#include "code/dancing-links-x.cpp"

int main() {
	mt19937 rng(6);
	for (int it = 0; it < 30000; ++it) {
		int m = 1 + rng() % 7, rows = rng() % 10;
		vector<vector<int>> M(rows);
		for (auto &row : M) for (int c = 0; c < m; ++c) if (rng() % 3 == 0) row.push_back(c);
		init(m);
		for (int i = 0; i < rows; ++i) insert(i, M[i]);
		auto exact = [&](const vector<int> &chosen) {
			vector<int> c(m);
			for (int i : chosen) for (int x : M[i]) c[x]++;
			return count(c.begin(), c.end(), 1) == m;
		};
		bool br = false;
		for (int mask = 0; mask < (1 << rows) && !br; ++mask) {
			vector<int> chosen;
			for (int i = 0; i < rows; ++i) if (mask >> i & 1) chosen.push_back(i);
			br = exact(chosen);
		}
		bool got = dlx();
		if (got != br || (got && !exact(res))) {
			printf("mismatch (m=%d): got %d want %d, rows:\n", m, got, br);
			for (auto &row : M) { for (int x : row) printf("%d ", x); puts(""); }
			return 1;
		}
	}
	puts("OK dancing-links-x");
}
