#include "check/prelude.h"
vector<vector<int>> ch; vector<int> val; // random game tree, val: leaf score for the player to move
struct state {
	int id;
	bool finished() { return ch[id].empty(); }
	int score() { return val[id]; }
	vector<state> next() { vector<state> r; for (int c : ch[id]) r.push_back({c}); return r; }
};
#include "code/alphabeta.cpp"

int negamax(int u) { if (ch[u].empty()) return val[u]; int b = INT_MIN; for (int c : ch[u]) b = max(b, -negamax(c)); return b; }

int main() {
	unsigned seed = 10; mt19937 rng(seed);
	for (int it = 0; it < 3000; ++it) {
		ch.assign(1, {}); val.assign(1, 0);
		for (int i = 0; i < (int)ch.size() && ch.size() < 300; ++i) {
			int k = (i == 0 || rng() % 3) ? rng() % 4 : 0;
			for (int j = 0; j < k; ++j) ch[i].push_back(ch.size()), ch.emplace_back(), val.push_back((int)(rng() % 21) - 10);
		}
		int want = negamax(0), got = alphabeta({0}, -1000, 1000);
		if (want != got) { printf("seed %u it %d: want %d got %d\n", seed, it, want, got); return 1; }
	}
	puts("OK alphabeta");
}
