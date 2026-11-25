// Recursive descent, O(n): non-negative integers, + - * / (left assoc.), unary -, parentheses,
// spaces. cal(x, y, op) applies a binary op (user-defined, e.g. modular). parse("2*(3+4)-10/2") = 9
struct Expr {
	string s;
	int i = 0;
	int get(int lv) { // lv 0: + -, 1: * /, 2: unary - and atoms
		if (lv == 2) {
			if (s[i] == '-') return i++, -get(2);
			int v = 0;
			if (s[i] == '(') return i++, v = get(0), i++, v;
			while (isdigit(s[i])) v = v * 10 + s[i++] - '0';
			return v;
		}
		int v = get(lv + 1);
		for (char o; (o = s[i]) && strchr(lv ? "*/" : "+-", o);) i++, v = cal(v, get(lv + 1), o);
		return v;
	}
};
int parse(string s) { return s.erase(remove(all(s), ' '), s.end()), Expr{s}.get(0); }
