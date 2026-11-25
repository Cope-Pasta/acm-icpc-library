const double eps = 1e-8, pi = acos(-1.0);
int dblcmp(double x) { return x < -eps ? -1 : x > eps; }
struct point3_t {
	double x, y, z;
	point3_t(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
	point3_t operator+(const point3_t &o) const { return point3_t(x + o.x, y + o.y, z + o.z); }
	point3_t operator-(const point3_t &o) const { return point3_t(x - o.x, y - o.y, z - o.z); }
	point3_t operator*(double k) const { return point3_t(x * k, y * k, z * k); }
	point3_t operator/(double k) const { return point3_t(x / k, y / k, z / k); }
	double length() const { return sqrt(x * x + y * y + z * z); }
};
double dist(point3_t a, point3_t b) { return (a - b).length(); }
