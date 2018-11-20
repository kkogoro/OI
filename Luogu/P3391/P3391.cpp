#include <iostream>
#include <cstdio>


inline int read() {
	int x = 0, f = 1;
	char ch = getchar();
	while (ch < '0' || ch > '9') {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (ch >= '0' && ch <= '9') {
		x = x * 10  + ch - '0';
		ch = getchar();
	}
	return x * f;
}


const int MAXN = 100007;

struct Splay {
	struct Node {
		int fa, ch[2], sum, tag;
	} node[MAXN];

#define F(x) node[x].fa
#define H(x, y) node[x].ch[y]
#define S(x) node[x].sum
#define L(x) node[x].ch[0]
#define R(x) node[x].ch[1]
#define root node[0].ch[1]
#define T(x) node[x].tag
	
	inline void pushUP(int x) {
		S(x) = S(L(x)) + S(R(x)) + 1;
		return;
	}
	inline void pushDOWN(int x) {
		if (x == 0) return;
		if (T(x)) {
			std::swap(L(x), R(x));
			T(L(x)) ^= 1;
			T(R(x)) ^= 1;
			T(x) = 0;
		}
		return;
	}
	inline int indent(int x) {
		return H(F(x), 0) == x ? 0 : 1;	
	}
	inline void connect(int x, int f, int how) {
		F(x) = f, H(f, how) = x;
		return;	
	}
	inline void rotate(int x) {
		int y = F(x), r = F(F(x));
		int yson = indent(x), rson = indent(y);
		connect(H(x, yson ^ 1), y, yson);
		connect(y, x, yson ^ 1);
		connect(x, r, rson);
		pushUP(y);
		pushUP(x);
		return;	
	}
	inline void splay(int x, int aim) {
		int Faim = F(aim);
		while (F(x) != Faim) {
			int y = F(x);
			if (y == aim) rotate(x);
			else if (indent(x) == indent(y)) rotate(y), rotate(x);
			else rotate(x), rotate(x);
		}
		return;
	}
	inline int getNum(int aim) {
		int p = root;
		while (1) {
			pushDOWN(p);
			if (S(L(p)) + 1 == aim) {
				return p;
			}
			else if (S(L(p)) >= aim) {
				p = L(p);	
			}
			else {
				aim -= S(L(p)) + 1;
				p = R(p);
			}
		}
	}	
	inline void rever(int l, int r) {
		int x = getNum(l);
		int y = getNum(r + 2);
		splay(x, root);
		splay(y, R(root));
		int z = L(y);
		T(z) ^= 1;
		return;
	}
	int build(int l, int r, int f) {
		if (l > r) return 0;
		int mid = (l + r) >> 1;
		F(mid) = f;
		L(mid) = build(l, mid - 1, mid);
		R(mid) = build(mid + 1, r, mid);
		pushUP(mid);
		return mid;	
	}
	void print(int p, int n) {
		pushDOWN(p);
		if (L(p)) print(L(p), n);
		if (p > 1 && p < n + 2) printf("%d ", p - 1);
		if (R(p)) print(R(p), n);
		return;	
	}
	
#undef F
#undef H
#undef S
#undef L
#undef R
#undef root
#undef T

} tree;

int main() {
	int n = read();
	int m = read();
	tree.build(1, n + 2, 0);
	tree.node[0].ch[1] = (1 + n + 2) >> 1;
	for (int i = 1; i <= m; ++i) {
		int l = read();
		int r = read();
		tree.rever(l, r);	
	}
	tree.print(tree.node[0].ch[1], n);
	return 0;
}
