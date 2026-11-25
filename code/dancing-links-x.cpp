// Dancing links (Algorithm X), exact cover. DLX d(m): m columns, 0-indexed.
// d.add(row, cols): cols of the 1s in row.
// d.solve(): true iff some rows cover every column exactly once (rows in d.res).
struct DLX {
	vector<int> L, R, U, D, C, A, S, res; // node 0: header, node c + 1: column c
	DLX(int m) : S(m + 1) {
		for (int i = 0; i <= m; ++i) {
			L.push_back(i - 1), R.push_back(i + 1), U.push_back(i);
			D.push_back(i), C.push_back(i), A.push_back(0);
		}
		L[0] = m, R[m] = 0;
	}
	void add(int row, vector<int> t) {
		int f = L.size();
		for (int c : t) {
			int p = L.size();
			++S[++c], C.push_back(c), A.push_back(row), U.push_back(c);
			D.push_back(D[c]), U[D[c]] = p, D[c] = p;
			L.push_back(p), R.push_back(p);
			L[p] = L[f], R[p] = f, R[L[f]] = p, L[f] = p;
		}
	}
	void remove(int x) {
		L[R[x]] = L[x], R[L[x]] = R[x];
		for (int i = D[x]; i != x; i = D[i])
			for (int j = R[i]; j != i; j = R[j])
				U[D[j]] = U[j], D[U[j]] = D[j], --S[C[j]];
	}
	void resume(int x) {
		for (int i = U[x]; i != x; i = U[i])
			for (int j = L[i]; j != i; j = L[j])
				U[D[j]] = j, D[U[j]] = j, ++S[C[j]];
		L[R[x]] = x, R[L[x]] = x;
	}
	bool solve() {
		if (!R[0]) return true;
		int c = R[0];
		for (int i = R[0]; i; i = R[i])
			if (S[i] < S[c]) c = i;
		remove(c);
		for (int i = D[c]; i != c; i = D[i]) {
			res.push_back(A[i]);
			for (int j = R[i]; j != i; j = R[j]) remove(C[j]);
			if (solve()) return true;
			for (int j = L[i]; j != i; j = L[j]) resume(C[j]);
			res.pop_back();
		}
		resume(c);
		return false;
	}
};
