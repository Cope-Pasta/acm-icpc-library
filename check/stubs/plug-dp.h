const int mod = 1e9 + 7;
template<typename T> struct matrix_t { // interface of code/matrix.cpp plus * and power
	int n; vector<vector<T>> x;
	matrix_t(int n, T v = 0);
	matrix_t operator*(const matrix_t &o) const;
	matrix_t power(ll k) const;
};
