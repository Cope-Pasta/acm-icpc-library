#include "check/prelude.h"
int cal(int x, int y, char op) { return op == '+' ? x + y : op == '-' ? x - y : op == '*' ? x * y : x / y; }
#include "code/expression-parse.cpp"

mt19937 rng(5);
// returns the expression and its value, evaluated recursively (never divides by 0)
pair<string, int> gen(int d) {
	if (d == 0 || rng() % 3 == 0) { int v = rng() % 20; return {to_string(v), v}; }
	int t = rng() % 6;
	if (t == 0) { auto e = gen(d - 1); return {"(" + e.first + ")", e.second}; }
	if (t == 1) { auto e = gen(d - 1); return {"(-(" + e.first + "))", -e.second}; }
	// left-assoc chains of same precedence would need care; wrap operands in parentheses
	auto a = gen(d - 1), b = gen(d - 1);
	char op = "+-*/"[rng() % 4];
	if (op == '/' && b.second == 0) op = '+';
	return {"(" + a.first + ")" + op + "(" + b.first + ")", cal(a.second, b.second, op)};
}

int main() {
	for (int it = 0; it < 100000; ++it) {
		auto e = gen(4);
		if (parse(e.first) != e.second) { printf("mismatch: %s got %d want %d\n", e.first.c_str(), parse(e.first), e.second); return 1; }
	}
	vector<pair<string, int>> fixed = {{"2*(3+4)-10/2", 9}, {"-(2+3)*2", -10}, {"1-2-3", -4}, {"100/10/5", 2}, {"2+3*4", 14}, {"7", 7}, {"-7", -7}, {"-2-3", -5}, {"(-11)-(-19)", 8}, {"2*3-4*5", -14}};
	for (auto &[s, v] : fixed) if (parse(s) != v) { printf("mismatch: %s got %d want %d\n", s.c_str(), parse(s), v); return 1; }
	puts("OK expression-parse");
}
