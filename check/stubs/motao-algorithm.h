struct query_t { int l, r, ID; };
vector<query_t> Q; vector<int> ans; int res;
bool cmpL(const query_t &a, const query_t &b);
bool cmpR(const query_t &a, const query_t &b);
void clear(); void add(int pos, int d);
void motao() {
