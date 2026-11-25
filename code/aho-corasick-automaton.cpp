// Aho-Corasick over 'a'..'a'+C-1, node 0 = root, O(C * total length).
// After build(): ch[p][c] is the full transition function,
// flag[p] = some pattern is a suffix of the string read to reach p.
struct AC {
	vector<array<int, C>> ch{{}};
	vector<int> fail{0};
	vector<bool> flag{0};
	void add(const string &s) {
		int p = 0;
		for (char c : s) {
			if (!ch[p][c - 'a']) {
				ch[p][c - 'a'] = ch.size();
				ch.emplace_back(), fail.push_back(0), flag.push_back(0);
			}
			p = ch[p][c - 'a'];
		}
		flag[p] = 1;
	}
	void build() {
		for (queue<int> q({0}); q.size(); q.pop()) {
			int p = q.front();
			flag[p] = flag[p] || flag[fail[p]];
			for (int i = 0; i < C; i++) {
				int &v = ch[p][i];
				if (v) fail[v] = p ? ch[fail[p]][i] : 0, q.push(v);
				else v = ch[fail[p]][i];
			}
		}
	}
};
