struct Node {
	int s, d;
	Node *left, *right;
} *nil;
deque<Node> pool; // deque: push_back keeps pointers valid
vector<Node*> root; // root[i]: version i, positions 0..n-1
vector<int> a;

Node *newNode(int _d, int _s, Node *_left, Node *_right) {
	pool.push_back({_s, _d, _left, _right});
	return &pool.back();
}

Node *build(int l, int r) {
	if (l == r) return newNode(0, a[l], nil, nil);
	int mid = (l + r) / 2;
	Node *nl = build(l, mid), *nr = build(mid + 1, r);
	return newNode(0, nl->s + nr->s, nl, nr);
}

void init() { // ask/add on [0, n - 1]
	pool.clear(); nil = newNode(0, 0, NULL, NULL);
	root = {build(0, a.size() - 1)};
}

void push(Node *node, int l, int r) {
	if (l == r) {
		node->d = 0;
	} else {
		if (node->d == 0) return;
		int mid = (l + r) / 2;
		Node *nl = newNode(node->left->d + node->d, node->left->s + node->d * int(mid - l + 1), node->left->left, node->left->right);
		Node *rl = newNode(node->right->d + node->d, node->right->s + node->d * int(r - mid), node->right->left, node->right->right);
		node->d = 0;
		node->left = nl;
		node->right = rl;
	}
}

int ask(Node *node, int l, int r, int ql, int qr) {
	push(node, l, r);
	if (l == ql && r == qr) return node->s;
	int mid = (l + r) / 2;
	if (qr <= mid) return ask(node->left, l, mid, ql, qr);
	else if (ql > mid) return ask(node->right, mid + 1, r, ql, qr);
	else return ask(node->left, l, mid, ql, mid) + ask(node->right, mid + 1, r, mid + 1, qr);
}

Node *add(Node *node, int l, int r, int ql, int qr, int d) {
	push(node, l, r);
	if (l == ql && r == qr) return newNode(node->d + d, node->s + d * int(r - l + 1), node->left, node->right);
	int mid = (l + r) / 2;
	if (qr <= mid) {
		Node *nl = add(node->left, l, mid, ql, qr, d);
		return newNode(0, nl->s + node->right->s, nl, node->right);
	} else if (ql > mid) {
		Node *nr = add(node->right, mid + 1, r, ql, qr, d);
		return newNode(0, node->left->s + nr->s, node->left, nr);
	} else {
		Node *nl = add(node->left, l, mid, ql, mid, d);
		Node *nr = add(node->right, mid + 1, r, mid + 1, qr, d);
		return newNode(0, nl->s + nr->s, nl, nr);
	}
}