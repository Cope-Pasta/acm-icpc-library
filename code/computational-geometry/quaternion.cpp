struct quater_t { // w + v.x i + v.y j + v.z k; dot, cross: common-3d
	double w;
	point3_t v;
	quater_t operator*(const quater_t &q) const {
		return {w * q.w - dot(v, q.v), q.v * w + v * q.w + cross(v, q.v)};
	}
};
// rotate p by a around unit axis k (right-hand rule)
point3_t rotate(point3_t k, point3_t p, double a) {
	quater_t q{cos(a / 2), k * sin(a / 2)}, c{q.w, k * -sin(a / 2)};
	return (q * quater_t{0, p} * c).v;
}
