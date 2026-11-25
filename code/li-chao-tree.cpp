typedef long long ftype;
typedef complex<ftype> point;
#define x real
#define y imag

ftype f(point a,  ftype x) {
    return a.x() * x + a.y();
}

const int maxn = 2e5;

vector<point> line(4 * maxn, point(0, LLONG_MAX)); // root 0, children 2v+1, 2v+2

void add_line(point nw, int v = 0, int l = 0, int r = maxn) {
    int m = (l + r) / 2;
    bool lef = f(nw, l) < f(line[v], l);
    bool mid = f(nw, m) < f(line[v], m);
    if(mid) {
        swap(line[v], nw);
    }
    if(r - l == 1) {
        return;
    } else if(lef != mid) {
        add_line(nw, 2 * v + 1, l, m);
    } else {
        add_line(nw, 2 * v + 2, m, r);
    }
}

ftype get(int x, int v = 0, int l = 0, int r = maxn) {
    int m = (l + r) / 2;
    if(r - l == 1) {
        return f(line[v], x);
    } else if(x < m) {
        return min(f(line[v], x), get(x, 2 * v + 1, l, m));
    } else {
        return min(f(line[v], x), get(x, 2 * v + 2, m, r));
    }
}