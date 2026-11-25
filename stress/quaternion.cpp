#include "check/prelude.h"
#include "check/common/geo3d.h"
#include "code/computational-geometry/common-3d.cpp"
#include "code/computational-geometry/quaternion.cpp"

int main() {
	unsigned seed = 1; mt19937 rng(seed);
	auto rnd = [&](int l, int r) { return int(rng() % (r - l + 1)) + l; };
	auto rp = [&]() { return point3_t(rnd(-1000, 1000) / 100.0, rnd(-1000, 1000) / 100.0, rnd(-1000, 1000) / 100.0); };
	for (int it = 0; it < 20000; ++it) {
		point3_t k = rp(), p = rp();
		if (k.length() < 1e-3) continue;
		k = k / k.length(); // rotate() expects a unit axis
		double t = rnd(-62832, 62832) / 10000.0;
		point3_t got = rotate(k, p, t);
		point3_t exp = p * cos(t) + cross(k, p) * sin(t) + k * (dot(k, p) * (1 - cos(t))); // Rodrigues
		if ((got - exp).length() > 1e-9 * max(1.0, p.length())) {
			printf("seed %u it %d: axis (%g,%g,%g) p (%g,%g,%g) angle %g: got (%g,%g,%g) expected (%g,%g,%g)\n", seed, it,
				k.x, k.y, k.z, p.x, p.y, p.z, t, got.x, got.y, got.z, exp.x, exp.y, exp.z);
			return 1;
		}
	}
	puts("OK quaternion");
}
