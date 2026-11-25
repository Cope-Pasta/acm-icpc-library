#include "check/prelude.h"
#include "code/number-partition.cpp"

int main() { // vs coin-style dp; p(n) and the pentagonal partial sums fit in int for n <= 110
	vector<LL> dp(111); dp[0] = 1;
	for (int k = 1; k <= 110; ++k) for (int s = k; s <= 110; ++s) dp[s] += dp[s - k];
	for (int n = 0; n <= 110; ++n) if (partition(n) != dp[n]) { printf("n %d: want %lld got %d\n", n, dp[n], partition(n)); return 1; }
	puts("OK number-partition");
}
