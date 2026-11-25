#include "check/prelude.h"
const int maxn = 5000;
#include "code/splay.cpp"
// Brute force: the sequence as a vector of (node index, cnt), with a sentinel node at each end.
typedef vector<pair<int, int>> seq_t;
void dump(node_t *t, seq_t &out) {
	if (t == nil) return;
	dump(t->ch[0], out), out.push_back({int(t - s), t->cnt}), dump(t->ch[1], out);
}
int main() {
	mt19937 rng(1);
	for (int it = 0; it < 2000; ++it) {
		init();
		node_t *L = newNode(1); root = L; node_t *R = newNode(1); insert(L, R);
		seq_t b = {{1, 1}, {2, 1}};
		for (int op = 0; op < 60; ++op) {
			int ty = rng() % 5, units = 0;
			for (auto &e : b) units += e.second;
			if (ty == 0 || b.size() < 3) { // insert after any node but the right sentinel
				int i = rng() % (b.size() - 1), c = rng() % 5 + 1;
				node_t *x = newNode(c); insert(&s[b[i].first], x);
				b.insert(b.begin() + i + 1, {int(x - s), c});
			} else if (ty == 1) { // find the k-th unit
				int k = rng() % units, kk = k, i = 0;
				while (kk >= b[i].second) kk -= b[i++].second;
				if (find(root, k) - s != b[i].first) { printf("find mismatch it=%d op=%d\n", it, op); return 1; }
			} else if (ty == 2) { // split so that unit k is its own node (not a sentinel)
				int k = 1 + rng() % (units - 2), kk = k, i = 0;
				while (kk >= b[i].second) kk -= b[i++].second;
				int c = b[i].second, id = b[i].first, nxt = pt;
				if (findAndSplit(root, k) - s != id) { printf("findAndSplit mismatch it=%d\n", it); return 1; }
				seq_t mid;
				if (kk) mid.push_back({++nxt, kk});
				mid.push_back({id, 1});
				if (c - kk - 1) mid.push_back({++nxt, c - kk - 1});
				b.erase(b.begin() + i), b.insert(b.begin() + i, mid.begin(), mid.end());
			} else if (ty == 3) { // remove a non-sentinel node
				int i = 1 + rng() % (b.size() - 2);
				remove(&s[b[i].first]), b.erase(b.begin() + i);
			} else { // expose(x, y): the subtree holding exactly the nodes in [x, y]
				int i = 1 + rng() % (b.size() - 2), j = 1 + rng() % (b.size() - 2);
				if (i > j) swap(i, j);
				seq_t got; dump(expose(&s[b[i].first], &s[b[j].first]), got);
				if (got != seq_t(b.begin() + i, b.begin() + j + 1)) { printf("expose mismatch it=%d\n", it); return 1; }
			}
			seq_t got; dump(root, got);
			int units2 = 0; for (auto &e : b) units2 += e.second;
			if (got != b || root->size != units2) { printf("sequence mismatch it=%d op=%d ty=%d\n", it, op, ty); return 1; }
		}
	}
	puts("OK splay");
}
