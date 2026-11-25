// Persistent treap (rope of string chunks), O(log n) expected per op.
// Old versions stay valid. Empty tree = 0. split(t, k): (first k chars, rest).
// ins(t, p, v): insert v before position p. del(t, x, y): remove [x, y].
struct Node {
	Node *l, *r;
	string v;
	int y, s;
};
int sz(Node *t) { return t ? t->s : 0; }
Node *mk(Node *l, string v, Node *r, int y = rand()) {
	return new Node{l, r, v, y, sz(l) + (int)v.size() + sz(r)};
}
pair<Node *, Node *> split(Node *t, int k) {
	if (!t) return {};
	int a = sz(t->l), b = a + t->v.size();
	if (k <= a) {
		auto [x, y] = split(t->l, k);
		return {x, mk(y, t->v, t->r, t->y)};
	}
	if (k >= b) {
		auto [x, y] = split(t->r, k - b);
		return {mk(t->l, t->v, x, t->y), y};
	}
	return {mk(t->l, t->v.substr(0, k - a), 0, t->y),
	    mk(0, t->v.substr(k - a), t->r, t->y)};
}
Node *merge(Node *a, Node *b) {
	if (!a || !b) return a ? a : b;
	if (a->y > b->y) return mk(a->l, a->v, merge(a->r, b), a->y);
	return mk(merge(a, b->l), b->v, b->r, b->y);
}
Node *ins(Node *t, int p, string v) {
	auto [a, b] = split(t, p);
	return merge(merge(a, mk(0, v, 0)), b);
}
Node *del(Node *t, int x, int y) {
	auto [a, b] = split(t, x);
	return merge(a, split(b, y - x + 1).second);
}
