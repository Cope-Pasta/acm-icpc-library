#include "check/prelude.h"
#include "code/splay.cpp"
// Brute force: the sequence as a vector of (node index, cnt), with a sentinel node at each end.
typedef vector<pair<int, int>> seq_t;
Splay T;
void dump(int t, seq_t &out) {
	if (!t) return;
	dump(T.ch[t][0], out), out.push_back({t, T.cnt[t]}), dump(T.ch[t][1], out);
}
int main() {
	mt19937 rng(1);
	for (int it = 0; it < 2000; ++it) {
		T = Splay();
		T.root = T.add(1); T.ins(T.root, T.add(1));
		seq_t b = {{1, 1}, {2, 1}};
		for (int op = 0; op < 60; ++op) {
			int ty = rng() % 6, units = 0;
			for (auto &e : b) units += e.second;
			if (ty == 0 || b.size() < 3) { // insert after (d=1) / before (d=0) a node, staying between the sentinels
				int d = rng() % 2, i = d ? rng() % (b.size() - 1) : 1 + rng() % (b.size() - 1), c = rng() % 5 + 1;
				int x = T.add(c); T.ins(b[i].first, x, d);
				b.insert(b.begin() + i + d, {x, c});
			} else if (ty == 1) { // find the k-th unit
				int k = rng() % units, kk = k, i = 0;
				while (kk >= b[i].second) kk -= b[i++].second;
				if (T.find(k) != b[i].first) { printf("find mismatch it=%d op=%d\n", it, op); return 1; }
			} else if (ty == 2) { // split so that unit k is its own node (not a sentinel)
				int k = 1 + rng() % (units - 2), kk = k, i = 0;
				while (kk >= b[i].second) kk -= b[i++].second;
				int c = b[i].second, id = b[i].first, nxt = T.p.size() - 1;
				if (T.split(k) != id) { printf("split mismatch it=%d\n", it); return 1; }
				seq_t mid;
				if (kk) mid.push_back({++nxt, kk});
				mid.push_back({id, 1});
				if (c - kk - 1) mid.push_back({++nxt, c - kk - 1});
				b.erase(b.begin() + i), b.insert(b.begin() + i, mid.begin(), mid.end());
			} else if (ty == 3) { // remove a single non-sentinel node
				int i = 1 + rng() % (b.size() - 2);
				T.cut(b[i].first, b[i].first), b.erase(b.begin() + i);
			} else if (ty == 4) { // remove a range of non-sentinel nodes
				int i = 1 + rng() % (b.size() - 2), j = 1 + rng() % (b.size() - 2);
				if (i > j) swap(i, j);
				T.cut(b[i].first, b[j].first), b.erase(b.begin() + i, b.begin() + j + 1);
			} else { // expose(x, y): the subtree holding exactly the nodes in [x, y]
				int i = 1 + rng() % (b.size() - 2), j = 1 + rng() % (b.size() - 2);
				if (i > j) swap(i, j);
				seq_t got; dump(T.expose(b[i].first, b[j].first), got);
				if (got != seq_t(b.begin() + i, b.begin() + j + 1)) { printf("expose mismatch it=%d\n", it); return 1; }
			}
			seq_t got; dump(T.root, got);
			int units2 = 0; for (auto &e : b) units2 += e.second;
			if (got != b || T.sz[T.root] != units2 || T.p[T.root]) { printf("sequence mismatch it=%d op=%d ty=%d\n", it, op, ty); return 1; }
			for (int i = 0; i + 1 < (int)b.size(); ++i) // nb in both directions
				if (T.nb(b[i].first, 1) != b[i + 1].first || T.nb(b[i + 1].first, 0) != b[i].first) { printf("nb mismatch it=%d op=%d\n", it, op); return 1; }
		}
	}
	puts("OK splay");
}
