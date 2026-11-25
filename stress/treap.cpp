#include "check/prelude.h"
extern "C" const char *__asan_default_options() { return "detect_leaks=0"; } // nodes are never freed
#include "code/treap.cpp"
// Brute force: a vector in in-order (implicit key = position). unite is not tested: it
// splits by position although it is meant to unite by key.
void dump(pitem t, vector<int> &out) { if (t) dump(t->l, out), out.push_back(t->key), dump(t->r, out); }
bool sizes(pitem t) { return !t || (t->cnt == 1 + cnt(t->l) + cnt(t->r) && sizes(t->l) && sizes(t->r)); }
int main() {
	mt19937 gen(3);
	for (int it = 0; it < 2000; ++it) {
		pitem root = nullptr; vector<int> b;
		for (int op = 0; op < 80; ++op) {
			int ty = gen() % 3, n = b.size();
			if (ty == 0 || n == 0) { // insert v at position p
				int p = gen() % (n + 1), v = gen() % 100; pitem l, r;
				split(root, l, r, p), merge(l, l, new item(v)), merge(root, l, r);
				b.insert(b.begin() + p, v);
			} else if (ty == 1) { // erase [x, y)
				int x = gen() % n, y = x + 1 + gen() % (n - x); pitem l, m, r;
				split(root, l, r, x), split(r, m, r, y - x), merge(root, l, r);
				b.erase(b.begin() + x, b.begin() + y);
			} else { // split at k and check both halves, then merge back
				int k = gen() % (n + 1); pitem l, r; vector<int> gl, gr;
				split(root, l, r, k), dump(l, gl), dump(r, gr);
				if (gl != vector<int>(b.begin(), b.begin() + k) || gr != vector<int>(b.begin() + k, b.end())) { printf("split mismatch it=%d op=%d\n", it, op); return 1; }
				merge(root, l, r);
			}
			vector<int> got; dump(root, got);
			if (got != b || cnt(root) != (int)b.size() || !sizes(root)) { printf("mismatch it=%d op=%d\n", it, op); return 1; }
		}
	}
	puts("OK treap");
}
