// O(n log n) intersection of the half-planes left of a->b, in box |x|,|y| <= maxd.
// Returns the CCW polygon (an edge may degenerate to a point), {} if empty.
// isLL: common-2d
const double maxd = 1e5;
struct hp_t {
	point_t a, b;
	double ang() const { return atan2(b.y - a.y, b.x - a.x); }
};
bool out(hp_t h, point_t p) { return dblcmp(cross(h.b - h.a, p - h.a)) < 0; }
point_t isLL(hp_t g, hp_t h) { return isLL(g.a, g.b, h.a, h.b); }
vector<point_t> hpInter(vector<hp_t> h) {
	point_t c(maxd, -maxd);
	for (int k = 0; k < 4; ++k, c = c.rot90()) h.push_back({c, c.rot90()});
	sort(all(h), [](hp_t g, hp_t h) {
		return dblcmp(g.ang() - h.ang()) ? g.ang() < h.ang() : out(g, h.a);
	});
	vector<hp_t> q(h.size());
	int f = 0, e = 0; // deque q[f, e)
	for (int i = 0; i < (int)h.size(); ++i) {
		if (i && !dblcmp(h[i].ang() - h[i - 1].ang()))
			continue; // keep the innermost of parallel ones
		while (e - f > 1 && out(h[i], isLL(q[e - 2], q[e - 1]))) --e;
		while (e - f > 1 && out(h[i], isLL(q[f], q[f + 1]))) ++f;
		if (e - f && !dblcmp(cross(q[e - 1].b - q[e - 1].a, h[i].b - h[i].a)))
			return {}; // opposite ones met
		q[e++] = h[i];
	}
	while (e - f > 2 && out(q[f], isLL(q[e - 2], q[e - 1]))) --e;
	while (e - f > 2 && out(q[e - 1], isLL(q[f], q[f + 1]))) ++f;
	if (e - f < 3) return {};
	vector<point_t> r;
	for (int i = f; i < e; ++i) r.push_back(isLL(q[i], q[i + 1 < e ? i + 1 : f]));
	return r;
}
