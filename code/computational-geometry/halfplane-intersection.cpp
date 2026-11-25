// O(N^2) sol, polygon counterclockwise order
// i.e., left side of vector v1->v2 is the valid half plane
const double maxd = 1e5;
vector<point_t> p;

void init() { // order reversed if right side
	p = {point_t(-maxd, -maxd), point_t(maxd, -maxd), point_t(maxd, maxd), point_t(-maxd, maxd)};
}

void cut(point_t p1, point_t p2) {
	int n = p.size();
	vector<point_t> tp;
	for (int i = 0; i < n; ++i) {
		point_t a = p[i], b = p[(i + 1) % n];
		double v1 = cross(p2 - p1, a - p1);
		double v2 = cross(p2 - p1, b - p1);
		if (dblcmp(v1) >= 0) tp.push_back(a); // <= if right side
		if (dblcmp(v1) * dblcmp(v2) < 0) tp.push_back(isLL(p1, p2, a, b));
	}
	p = tp;
}