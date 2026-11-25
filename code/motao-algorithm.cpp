// Complexity: Q*N^0.5 * O(add), Q: vector of queries [l, r], positions from 0
int q = Q.size(), SQRTN = max(1, (int)sqrt((double)q));
sort(all(Q), cmpL);
for (int i = 0; i < q; i += SQRTN) {
	clear();
	int end = min(q, i + SQRTN), L = 0, R = -1;
	sort(Q.begin() + i, Q.begin() + end, cmpR);
	for (int j = i; j < end; ++j) {
		while (R < Q[j].r) add(++R, 1);
		while (L < Q[j].l) add(L++, -1);
		while (L > Q[j].l) add(--L, 1);
		ans[Q[j].ID] = res;
	}
}