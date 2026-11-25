int ind, color; // vertices 0..n-1, dfn[u] = 0 if unvisited, timer starts at 1
vector<int> dfn, low, block; vector<bool> instack; vector<vector<int>> edge; stack<int> sk;

void tarjan(int u) {
	dfn[u] = low[u] = ++ind;
	sk.push(u), instack[u] = true; 
	for (int v : edge[u]) { // Check if v is the parent of u(BCC)
		if (!dfn[v]) {
			tarjan(v);
			low[u] = min(low[u], low[v]);
			// Check BCC here
			// Cut vertex: Pop stack until v is poped, then add u.
		} else if (instack[v]) { // ! instack[v] -> else
			low[u] = min(low[u], dfn[v]);
		}
	}
	/* Check SCC here
	if (dfn[u] == low[u]) {
		++color; int v;
		do {
			v = sk.top(), instack[v] = false;
			sk.pop();
			block[v] = color;
		} while (v != u);
	} */
}