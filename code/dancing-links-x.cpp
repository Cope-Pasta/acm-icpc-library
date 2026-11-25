// node 0: header, node c + 1: column c (columns 0-indexed), res: rows of the found cover
vector<int> L, R, U, D, C, A, S, O, res;

void init(int m) { // m: number of columns
	for (auto *v : {&L, &R, &U, &D, &C, &A}) v->assign(m + 1, 0);
	S.assign(m + 1, 0), O.clear(), res.clear();
	for (int i = 0; i <= m; ++i) L[i] = i - 1, R[i] = i + 1, C[i] = D[i] = U[i] = i;
	L[0] = m, R[m] = 0;
}

void insert(int row, const vector<int> &t) { // t: columns of 1
	int first = L.size();
	for (int col : t) {
		int pt = L.size(); ++col, ++S[col];
		C.push_back(col), A.push_back(row), U.push_back(col), D.push_back(D[col]);
		U[D[col]] = pt, D[col] = pt;
		L.push_back(pt), R.push_back(pt);
		if (pt != first) L[pt] = pt - 1, R[pt] = first, R[pt - 1] = L[first] = pt;
	}
}

void remove(int x) {
	L[R[x]] = L[x], R[L[x]] = R[x];
	for (int i = D[x]; i != x; i = D[i]) {
		for (int j = R[i]; j != i; j = R[j]) {
			U[D[j]] = U[j], D[U[j]] = D[j], --S[C[j]];
		}
	}
}

void resume(int x) {
	for (int i = U[x]; i != x; i = U[i]) {
		for (int j = L[i]; j != i; j = L[j]) {
			U[D[j]] = j, D[U[j]] = j, ++S[C[j]];
		}
	}
	L[R[x]] = x, R[L[x]] = x;
}

bool dlx() {
	if (R[0] == 0) {
		res = O;
		return true;
	} else {
		int val = INT_MAX, col = 0;
		for (int i = R[0]; i != 0; i = R[i]) {
			if (S[i] <= val) col = i, val = S[i];
		}
		if (val == 0) return false;
		remove(col);
		for (int i = D[col]; i != col; i = D[i]) {
			O.push_back(A[i]);
			for (int j = R[i]; j != i; j = R[j]) remove(C[j]);
			if (dlx()) return true;
			for (int j = L[i]; j != i; j = L[j]) resume(C[j]);
			O.pop_back();
		}
		resume(col);
		return false;
	}
}
