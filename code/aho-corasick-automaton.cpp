struct trie_t { // node 0 is the root
	vector<array<int, C>> ch; vector<int> fail; vector<bool> flag;
	trie_t() : ch(1), fail(1), flag(1) {}
	void add(const string &str) {
		int p = 0;
		for (char c : str) {
			int x = c - 'a'; // fixed to [0, C - 1], C = |SIGMA|
			if (!ch[p][x]) ch[p][x] = ch.size(), ch.emplace_back(), fail.push_back(0), flag.push_back(false);
			p = ch[p][x];
		}
		flag[p] = true;
	}
	void build() {
		queue<int> q;
		for (int i = 0; i < C; ++i) if (ch[0][i]) q.push(ch[0][i]);
		while (!q.empty()) {
			int p = q.front(); q.pop();
			flag[p] = flag[p] || flag[fail[p]];
			for (int i = 0; i < C; ++i) {
				int &v = ch[p][i];
				if (v) fail[v] = ch[fail[p]][i], q.push(v);
				else v = ch[fail[p]][i];
			}
		}
	}
};
