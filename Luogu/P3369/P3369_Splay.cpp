#include <iostream>
#include <cstdio>



inline int read() {
	int x = 0, f = 1;
	char ch = getchar();
	while (ch < '0' ||ch > '9') {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (ch >= '0' && ch <= '9') {
		x = x * 10 + ch - '0';
		ch = getchar();
	}
	return x * f;
}


struct Splay {
	static const int MAXN = 100007;
	static const int INF = 100000000;	

	struct Node {
		int fa, val, rec, sum, ch[2];	
	} node[MAXN];
	int cntNode;
#define F(x) node[x].fa
#define V(x) node[x].val
#define C(x) node[x].rec
#define S(x) node[x].sum
#define L(x) node[x].ch[0]
#define R(x) node[x].ch[1]
#define H(x, y) node[x].ch[y]
#define root node[0].ch[1]

	inline void pushUP(int x) {
		S(x) = S(L(x)) + S(R(x)) + C(x);
		return;
	}
	inline int newNode(int val, int f) {
		++cntNode;
		V(cntNode) = val;
		C(cntNode) = S(cntNode) = 1;
		F(cntNode) = f;
		return cntNode;
	}
	inline void connect(int x, int f, int how) {
		H(f, how) = x, F(x) = f;
		return;
	}
	inline int indent(int x) {
		return L(F(x)) == x ? 0 : 1;
	}
	inline void rotate(int x) {
		int y = F(x), r = F(y);
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
	inline void insert(int val) {
		int p = root;
		if (root == 0) {
			root = newNode(val, 0);	
			return;
		}
		
		while (1) {
			++S(p);
			if (V(p) == val) {
				++C(p);
				splay(p, root);
				break;
			}
			
			int to = val < V(p) ? 0 : 1;
			if (H(p, to) == 0) {
				int born = newNode(val, p);
				H(p, to) = born;
				splay(born, root);
				break;	
			}
			p = H(p, to);
		}
		return;
	}
	
	inline int find(int val) {
		int p = root;
		while (1) {
			if (p == 0) return 0;
			if (V(p) == val) {
				splay(p, root);
				return p;
			}
			int to = val < V(p) ? 0 : 1;
			p = H(p, to);
		}
	}
	inline void erase(int val) {
		int p = find(val);
		if (p == 0) return;
		if (C(p) > 1) {
			--C(p);
			pushUP(p);
			return;
		}
		
		if (L(p) == 0 && R(p) == 0) {
			root = 0;
		}
		else if (L(p) == 0) {
			root = R(p);
			F(root) = 0;
		}
		else {
			int left = L(p);
			while (R(left)) left = R(left);
			splay(left, L(p));
			connect(R(root), left, 1);
			connect(left, 0, 1);
			pushUP(left);
		}
		return;
	}
	inline int getRank(int x) {
		return S(L(find(x))) + 1;
	}
	inline int getNum(int x) {
		int p = root;
		while (1) {
			if (S(L(p)) >= x) {
				p = L(p);
			}
			else if (S(L(p)) + C(p) >= x) {
				break;
			}
			else {
				x -= S(L(p)) + C(p);
				p = R(p);	
			}
		}
		splay(p, root);
		return V(p);
	}
	inline int lower(int x) {
		int p = root, ans = -INF;
		while (p) {
			if (V(p) < x) ans = std::max(ans, V(p));
			p = H(p, x <= V(p) ? 0 : 1);
		}
		return ans;
	}
	inline int upper(int x) {
		int p = root, ans = INF;
		while (p) {
			if (V(p) > x) ans = std::min(ans, V(p));
			p = H(p, x < V(p) ? 0 : 1);
		}
		return ans;
	}
#undef F
#undef V
#undef C
#undef S
#undef L
#undef R
#undef H
#undef root
} tree;


int main() {
	int n = read();
	while (n--) {
		int opt = read();
		int x = read();
		switch (opt) {
            case 1 :
                tree.insert(x);
                break;
            case 2 :
                tree.erase(x);
                break;
            case 3 :
                printf("%d\n", tree.getRank(x));
                break;
            case 4 :
                printf("%d\n", tree.getNum(x));
                break;
            case 5 :
                printf("%d\n", tree.lower(x));
                break;
            case 6 :
                printf("%d\n", tree.upper(x));
                break;
        }
	}
	return 0;
}
