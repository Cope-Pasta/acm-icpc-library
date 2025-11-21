mt19937 rng{583427}; // any seed, crucial when testing on windows
struct item {
    int key, prior;
    item *l, *r;
    item () {}
    item (int key) : key(key), prior(rng()), l(nullptr), r(nullptr) { }
    item (int key, int prior) : key(key), prior(prior), l(nullptr), r(nullptr) { }
};
typedef item* pitem;

int cnt (pitem t) {
    return t ? t->cnt : 0;
}

void push (pitem p) {} // just in case

void pull (pitem t) {
    if (t)
        t->cnt = 1 + cnt(t->l) + cnt (t->r);
}

void split (pitem t, pitem & l, pitem & r, int key) {
    if (!t)
        return void( l = r = 0 );
    push(t);
    int lsize = cnt(t->l); //implicit key
    if (key <= lsize)
        split (t->l, l, t->l, key),  r = t;
    else
        split (t->r, t->r, r, key - 1 - lsize),  l = t;
    pull(t);
}

void merge (pitem & t, pitem l, pitem r) {
    push(l);
    push(r);
    if (!l || !r)
        t = l ? l : r;
    else if (l->prior > r->prior)
        merge(l->r, l->r, r),  t = l;
    else
        merge(r->l, l, r->l),  t = r;
    pull(t);
}

pitem unite (pitem l, pitem r) {
    if (!l || !r)  return l ? l : r;
    push(l);
    push(r);
    if (l->prior < r->prior) swap(l, r);
    pitem lt, rt;
    split(r, l->key, lt, rt);
    l->l = unite(l->l, lt);
    l->r = unite(l->r, rt);
	pull(l);
    return l;
}
