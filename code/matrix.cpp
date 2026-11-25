template<typename T>
struct matrix_t {
	int n; vector<vector<T>> x;
	matrix_t(int n, T v = 0) : n(n), x(n, vector<T>(n)) {
		for (int i = 0; i < n; ++i) x[i][i] = v;
	}
	T determinant() const {
		T r = 1; vector<vector<T>> det = x;
		for (int i = 0; i < n; ++i) {
			for (int j = i + 1; j < n; ++j) {
				while (det[j][i] != 0) { // exact: only for integer/modular T; for doubles pick the max pivot and compare with eps
					T ratio = det[i][i] / det[j][i];
					for (int k = i; k < n; ++k) {
						det[i][k] -= ratio * det[j][k];
						swap(det[i][k], det[j][k]);
					}
					r = -r;
				}
			}
			r = r * det[i][i];
		}
		return r;
	}
};
