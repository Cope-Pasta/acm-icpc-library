#include "check/prelude.h"
const int C = 3;
#include "code/aho-corasick-automaton.cpp"

int main() {
	mt19937 rng(2);
	for (int it = 0; it < 20000; ++it) {
		trie_t ac; vector<string> pats;
		int k = rng() % 6;
		for (int i = 0; i < k; ++i) {
			string p; int l = 1 + rng() % 4;
			for (int j = 0; j < l; ++j) p += 'a' + rng() % C;
			pats.push_back(p), ac.add(p);
		}
		ac.build();
		string t; int n = rng() % 20;
		for (int i = 0; i < n; ++i) t += 'a' + rng() % C;
		for (int i = 0, p = 0; i < n; ++i) { // flag: some pattern ends at t[i]
			p = ac.ch[p][t[i] - 'a'];
			bool br = false;
			for (auto &s : pats) if ((int)s.size() <= i + 1 && t.compare(i + 1 - s.size(), s.size(), s) == 0) br = true;
			if (ac.flag[p] != br) {
				printf("mismatch: text=\"%s\" pos=%d patterns:", t.c_str(), i);
				for (auto &s : pats) printf(" %s", s.c_str());
				puts(""); return 1;
			}
		}
	}
	puts("OK aho-corasick-automaton");
}
