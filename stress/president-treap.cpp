#include "check/prelude.h"
extern "C" const char *__asan_default_options() { return "detect_leaks=0"; } // nodes are never freed
#include "code/president-treap.cpp"
// Brute force: every version kept as a plain string; old versions must stay unchanged.
void dump(Node *t, string &out) { if (t) dump(t->l, out), out += t->v, dump(t->r, out); }
int main() {
	mt19937 gen(4); srand(4);
	for (int it = 0; it < 300; ++it) {
		vector<Node *> roots = {nullptr}; vector<string> b = {""};
		for (int op = 0; op < 60; ++op) {
			int ver = gen() % roots.size(), n = b[ver].size();
			string s = b[ver];
			if (gen() % 3 || n == 0) { // insert a random string before position p
				int p = gen() % (n + 1); string v;
				for (int k = gen() % 4 + 1; k--;) v += char('a' + gen() % 3);
				roots.push_back(ins(roots[ver], p, v)), s.insert(p, v);
			} else { // remove [x, y]
				int x = gen() % n, y = x + gen() % (n - x);
				roots.push_back(del(roots[ver], x, y)), s.erase(x, y - x + 1);
			}
			b.push_back(s);
			for (size_t v = 0; v < roots.size(); ++v) {
				string got; dump(roots[v], got);
				if (got != b[v] || sz(roots[v]) != (int)b[v].size()) { printf("mismatch it=%d op=%d version=%zu\n", it, op, v); return 1; }
			}
		}
	}
	puts("OK president-treap");
}
