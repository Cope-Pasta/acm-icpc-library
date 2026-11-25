#include "check/prelude.h"
#include "code/splitmix.cpp"

int main() { // reference values of SplitMix64 (state 0 -> 0xe220a8397b1dcdaf, ...)
	uint64_t s = 0, want[] = {0xe220a8397b1dcdafULL, 0x6e789e6aa1b965f4ULL, 0x06c45d188009454fULL};
	for (uint64_t w : want) {
		if (custom_hash::splitmix64(s) != w) { printf("splitmix64(%llu)\n", (unsigned long long)s); return 1; }
		s += 0x9e3779b97f4a7c15ULL;
	}
	custom_hash h; set<size_t> seen;
	for (uint64_t x = 0; x < 100000; ++x) { if (h(x) != h(x)) { puts("not deterministic"); return 1; } seen.insert(h(x)); }
	if (seen.size() != 100000) { puts("collision"); return 1; }
	unordered_map<uint64_t, int, custom_hash> mp; for (int i = 0; i < 1000; ++i) mp[(uint64_t)i << 20] = i;
	for (int i = 0; i < 1000; ++i) if (mp[(uint64_t)i << 20] != i) { puts("unordered_map"); return 1; }
	puts("OK splitmix");
}
