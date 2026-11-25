// n x n matrix, matrix_t(n, v) = v * I. determinant(): Euclid-style elimination with an exact zero test,
// so only for integer or modular T (for double, pick the max pivot and compare with eps). O(n^3 log C).
template <typename T>
struct matrix_t {
	int n;
	vector<vector<T>> x;
	matrix_t(int n, T v = 0) : n(n), x(n, vector<T>(n)) {
		for (int i = 0; i < n; ++i) x[i][i] = v;
	}
	T determinant() const {
		T r = 1;
		auto a = x;
		for (int i = 0; i < n; ++i) {
			for (int j = i + 1; j < n; ++j)
				while (a[j][i] != 0) {
					T t = a[i][i] / a[j][i];
					for (int k = i; k < n; ++k) a[i][k] -= t * a[j][k];
					swap(a[i], a[j]), r = -r;
				}
			r = r * a[i][i];
		}
		return r;
	}
};
