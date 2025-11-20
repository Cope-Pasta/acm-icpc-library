template<typename T, int N>
struct matrix_t {
	T x[N + 1][N + 1];
	matrix_t(T v) {
		memset(x, 0, sizeof(x));
		for (int i = 1; i <= N; ++i) x[i][i] = v;
	}
	T determinant() const {
		T r = 1, det[N + 1][N + 1];
		for (int i = 1; i <= N; ++i) memcpy(det[i], x[i], sizeof(x[i]));
		for (int i = 1; i <= N; ++i) {
			for (int j = i + 1; j <= N; ++j) {
				while (det[j][i] != 0) {
					T ratio = det[i][i] / det[j][i];
					for (int k = i; k <= N; ++k) {
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
