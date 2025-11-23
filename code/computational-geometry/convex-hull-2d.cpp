bool cmp(const point_t p1, const point_t p2) {
	return dblcmp(p1.y - p2.y) == 0 ? p1.x < p2.x : p1.y < p2.y;
}

int graham(vector<point_t>& p) { // Points co-lined are ignored.
	int top = 1; static point_t sk[maxn];
	sort(p, p + n, cmp);
	sk[0] = p[0], sk[1] = p[1];
	for (int i = 2; i < n; ++i) {
		while (top >= 1 && dblcmp(cross(p[i] - sk[top - 1], sk[top] - sk[top - 1])) >= 0) --top;
		sk[++top] = p[i];
	}
	int ttop = top;
	for (int i = n - 2; i >= 0; --i) {
		while (top > ttop && dblcmp(cross(p[i] - sk[top - 1], sk[top] - sk[top - 1])) >= 0) --top;
		sk[++top] = p[i];
	}
	for (int i = 0; i < top; ++i) p[i] = sk[i];
	return --top;
}
