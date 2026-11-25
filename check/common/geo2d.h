const double eps = 1e-8, pi = acos(-1.0);
int dblcmp(double x) { return x < -eps ? -1 : x > eps; }
double sqr(double x) { return x * x; }
struct point_t {
	double x, y;
	point_t(double x = 0, double y = 0) : x(x), y(y) {}
	point_t operator+(const point_t &o) const { return point_t(x + o.x, y + o.y); }
	point_t operator-(const point_t &o) const { return point_t(x - o.x, y - o.y); }
	point_t operator*(double k) const { return point_t(x * k, y * k); }
	point_t operator/(double k) const { return point_t(x / k, y / k); }
	point_t rot90() const { return point_t(-y, x); }
};
double cross(point_t a, point_t b) { return a.x * b.y - a.y * b.x; }
double dot(point_t a, point_t b) { return a.x * b.x + a.y * b.y; }
double dist(point_t a) { return sqrt(dot(a, a)); }
double dist(point_t a, point_t b) { return dist(a - b); }
double dist2(point_t a, point_t b) { return dot(a - b, a - b); }
