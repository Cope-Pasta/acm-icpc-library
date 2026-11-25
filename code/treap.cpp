// Implicit treap (sequence indexed by position), O(log n) expected per op.
// split(t, k, l, r): l = first k elements, r = rest.
// Lazy tags: push(t) at the top of split and merge.
mt19937 rng(583427);
struct Node {
	Node *l = 0, *r = 0;
	int val, y = rng(), c = 1;
	Node(int val) : val(val) {}
};
int cnt(Node *t) { return t ? t->c : 0; }
void pull(Node *t) { t->c = cnt(t->l) + cnt(t->r) + 1; }
void split(Node *t, int k, Node *&l, Node *&r) {
	if (!t) l = r = 0;
	else if (cnt(t->l) >= k) split(t->l, k, l, t->l), r = t, pull(t);
	else split(t->r, k - cnt(t->l) - 1, t->r, r), l = t, pull(t);
}
Node *merge(Node *l, Node *r) {
	if (!l || !r) return l ? l : r;
	if (l->y > r->y) return l->r = merge(l->r, r), pull(l), l;
	return r->l = merge(l, r->l), pull(r), r;
}
