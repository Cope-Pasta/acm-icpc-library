// O(NlogN) sol, Left is valid half plane. Note that the edge of hull may degenerate to a point.
struct hp_t {
	point_t p1, p2;
	double a;
	hp_t() {}
	hp_t(point_t tp1, point_t tp2) : p1(tp1), p2(tp2) {
		tp2 = tp2 - tp1;
		a = atan2(tp2.y, tp2.x);
	}
	bool operator==(const hp_t &r) const {
		return dblcmp(a - r.a) == 0;
	}
	bool operator<(const hp_t &r) const {
		if (dblcmp(a - r.a) == 0) return dblcmp(cross(r.p2 - r.p1, p2 - r.p1)) >= 0;
		else return a < r.a;
	}
};
vector<hp_t> hp;

void addhp(point_t p1, point_t p2) {
	hp.push_back(hp_t(p1, p2));
}

void init() {
	hp.clear();
	addhp(point_t(-maxd, -maxd), point_t(maxd, -maxd));
	addhp(point_t(maxd, -maxd), point_t(maxd, maxd));
	addhp(point_t(maxd, maxd), point_t(-maxd, maxd));
	addhp(point_t(-maxd, maxd), point_t(-maxd, -maxd));
}

bool checkhp(hp_t h1, hp_t h2, hp_t h3) {
	point_t p = isLL(h1.p1, h1.p2, h2.p1, h2.p2);
	return dblcmp(cross(p - h3.p1, h3.p2 - h3.p1)) > 0;
}

vector<point_t> hp_inter() {
	sort(all(hp));
	hp.erase(unique(all(hp)), hp.end());
	deque<hp_t> DQ;
	DQ.push_back(hp[0]);
	DQ.push_back(hp[1]);
	for (int i = 2; i < (int)hp.size(); ++i) {
		while (DQ.size() > 1 && checkhp(*-- --DQ.end(), *--DQ.end(), hp[i])) DQ.pop_back();
		while (DQ.size() > 1 && checkhp(*++DQ.begin(), *DQ.begin(), hp[i])) DQ.pop_front();
		if (dblcmp(cross(DQ.back().p2 - DQ.back().p1, hp[i].p2 - hp[i].p1)) == 0) return {}; // opposite half-planes met: empty
		DQ.push_back(hp[i]);
	}
	while (DQ.size() > 1 && checkhp(*-- --DQ.end(), *--DQ.end(), DQ.front())) DQ.pop_back();
	while (DQ.size() > 1 && checkhp(*++DQ.begin(), *DQ.begin(), DQ.back())) DQ.pop_front();
	if (DQ.size() < 3) return {};
	DQ.push_front(DQ.back());
	vector<point_t> res;
	while (DQ.size() > 1) {
		hp_t tmp = DQ.front();
		DQ.pop_front();
		res.push_back(isLL(tmp.p1, tmp.p2, DQ.front().p1, DQ.front().p2));
	}
	return res;
}
