#include "check/prelude.h"
// The snippet hard-codes a placeholder f(x) = 0. To integrate real functions, every `return E`
// in it becomes `return hook(x) + E`: inside f, x is its double parameter and hook calls F;
// elsewhere x is the global tag below and hook returns 0.
function<double(double)> F;
struct tag_t {} x;
double hook(double v) { return F(v); }
double hook(tag_t) { return 0; }
#define return return hook(x) +
#include "code/simpson-integration.cpp"
#undef return

int main() {
	struct { function<double(double)> f; double l, r, want; } cases[] = {
		{[](double t) { return 3 * t * t; }, 0, 2, 8},
		{[](double t) { return sin(t); }, 0, acos(-1.0), 2},
		{[](double t) { return exp(t); }, 0, 1, exp(1.0) - 1},
		{[](double t) { return 1 / (1 + t * t); }, 0, 1, acos(-1.0) / 4},
		{[](double t) { return pow(t, 5) - 2 * t; }, -1, 3, (729 - 1) / 6.0 - 8},
		{[](double t) { return sqrt(1 - t * t); }, -1, 1, acos(-1.0) / 2},
		{[](double) { return 0.0; }, 0, 5, 0},
	};
	for (auto &c : cases) {
		F = c.f; double got = simpson(c.l, c.r);
		if (fabs(got - c.want) > 1e-8) { printf("[%g, %g]: want %.12f got %.12f\n", c.l, c.r, c.want, got); return 1; }
	}
	puts("OK simpson-integration");
}
