#include "check/prelude.h"
extern "C" const char *__asan_default_options() { return "detect_leaks=0"; } // nodes are never freed
#include "code/treap.cpp"
// Brute force: a vector in in-order (implicit key = position).
void dump(Node *t, vector<int> &out) { if (t) dump(t->l, out), out.push_back(t->val), dump(t->r, out); }
bool sizes(Node *t) { return !t || (t->c == 1 + cnt(t->l) + cnt(t->r) && sizes(t->l) && sizes(t->r)); }
int main() {
	mt19937 gen(3);
	for (int it = 0; it < 2000; ++it) {
		Node *root = nullptr; vector<int> b;
		for (int op = 0; op < 80; ++op) {
			int ty = gen() % 3, n = b.size();
			if (ty == 0 || n == 0) { // insert v at position p
				int p = gen() % (n + 1), v = gen() % 100;
				Node *l, *r; split(root, p, l, r);
				root = merge(merge(l, new Node(v)), r);
				b.insert(b.begin() + p, v);
			} else if (ty == 1) { // erase [x, y)
				int x = gen() % n, y = x + 1 + gen() % (n - x);
				Node *l, *m, *r; split(root, x, l, r), split(r, y - x, m, r);
				root = merge(l, r);
				b.erase(b.begin() + x, b.begin() + y);
			} else { // split at k and check both halves, then merge back
				int k = gen() % (n + 1); vector<int> gl, gr;
				Node *l, *r; split(root, k, l, r);
				dump(l, gl), dump(r, gr);
				if (gl != vector<int>(b.begin(), b.begin() + k) || gr != vector<int>(b.begin() + k, b.end())) { printf("split mismatch it=%d op=%d\n", it, op); return 1; }
				root = merge(l, r);
			}
			vector<int> got; dump(root, got);
			if (got != b || cnt(root) != (int)b.size() || !sizes(root)) { printf("mismatch it=%d op=%d\n", it, op); return 1; }
		}
	}
	puts("OK treap");
}
