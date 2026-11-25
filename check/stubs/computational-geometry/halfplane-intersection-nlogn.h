#include "check/common/geo2d.h"
const double maxd = 1e5;
point_t isLL(point_t a, point_t b, point_t c, point_t d) { point_t u = b - a, v = d - c; return a + u * (cross(c - a, v) / cross(u, v)); }
