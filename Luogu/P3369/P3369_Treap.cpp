#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>


inline int read() {
	int x = 0, f = 1;
	char ch = getchar();
	while (ch < '0' || ch > '9') {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (ch >= '0' && ch <= '9') {
		x = x * 10 + ch - '0';
		ch = getchar();
	}
	return x * f;
}


struct Treap {
	static const int INF = 100000000;
	static const int MAXN = 100007;
	struct Node {
		int lc, rc, num, cnt, sum, dat;	
	} node[MAXN];
	int cntNode, root;
#define L(x) node[x].lc
#define R(x) node[x].rc
#define V(x) node[x].num
#define C(x) node[x].cnt
#define S(x) node[x].sum
#define D(x) node[x].dat
	inline int newNode(int val) {
		++cntNode;
		V(cntNode) = val;
		D(cntNode) = std::rand();
		C(cntNode) = S(cntNode) = 1;
		return cntNode;
	}
	inline void pushUP(int cur) {
		S(cur) = S(L(cur)) + S(R(cur)) + C(cur);
		return;
	}
	inline void build() {
		root = newNode(-INF);
		R(root) = newNode(INF);
		pushUP(root);
		return;
	}
	inline void zig(int &p) {
		int q = L(p);
		L(p) = R(q), R(q) = p, p = q;
		pushUP(R(p));
		pushUP(p);
		return;	
	}
	inline void zag(int &p) {
		int q = R(p);
		R(p) = L(q), L(q) = p, p = q;
		pushUP(L(p));
		pushUP(p);
		return;
	}
	void insert(int &p, int val) {
		if (!p) {
			p = newNode(val);
			return;	
		}
		if (val == V(p)) {
			++C(p);
			pushUP(p);
			return;	
		}
		if (val < V(p)) {
			insert(L(p), val);
			if (D(L(p)) > D(p)) {
				zig(p);	
			}
		}
		else {
			insert(R(p), val);
			if (D(R(p)) > D(p)) {
				zag(p);	
			}
		}
		pushUP(p);
		return;
	}
	void erase(int &p, int val) {
		if (!p) {
			return;
		}
		
		if (val == V(p)) {
			if (C(p) > 1) {
				--C(p);
			}
			else if (L(p) || R(p)) {
				if (!R(p) || D(L(p)) > D(R(p))) {
					zig(p);
					erase(R(p), val);
				}
				else {
					zag(p);
					erase(L(p), val);
				}
			}
			else {
				p = 0;
			}
		}
		else {
			val < V(p) ? erase(L(p), val) : erase(R(p), val);	
		}
		pushUP(p);
		return;
	}
	int getRank(int &p, int val) {
		if (!p) {
			return 0;
		}
		if (val == V(p)) {
			return S(L(p)) + 1;	
		}
		if (val < V(p)) {
			return getRank(L(p), val);
		}
		return S(L(p)) + C(p) + getRank(R(p), val);
	}
	int getNum(int &p, int rank) {
		if (!p) {
			return INF;
		}
		if (S(L(p)) >= rank) {
			return getNum(L(p), rank);	
		}
		if (S(L(p)) + C(p) >= rank) {
			return V(p);
		}
		return getNum(R(p), rank - S(L(p)) - C(p));
	}
	int getPre(int val) {
		int ans = 0, p = root;
		while (p > 0) {
			if (val > V(p)) {
				ans = p;
				p = R(p);	
			}
			else {
				p = L(p);
			}
		}
		return V(ans);
	}
	int getNext(int val) {
		int ans = 0, p = root;
		while (p > 0) {
			if (val < V(p)) {
				ans = p;
				p = L(p);
			}
			else {
				p = R(p);
			}
		}
		return V(ans);
	}
#undef L
#undef R
#undef V
#undef C
#undef S
#undef D
} tree_1;


namespace BST {
	inline void init() {
		tree_1.build();
		return;
	}
	inline void insert(int val) {
		tree_1.insert(tree_1.root, val);
		return;
	}
	inline void erase(int val) {
		tree_1.erase(tree_1.root, val);
		return;
	}
	inline int getRank(int val) {
		return tree_1.getRank(tree_1.root, val) - 1;
	}
	inline int getNum(int rank) {
		return tree_1.getNum(tree_1.root, rank + 1);
	}
	inline int getPre(int val) {
		return tree_1.getPre(val);
	}
	inline int getNext(int val) {
		return tree_1.getNext(val);	
	}
}


int main() {
	srand(time(NULL));
	
	BST::init();
	int n = read();
	while (n--) {
		int opt = read();
		int x = read();
		switch(opt) {
            case 1 :
                BST::insert(x);
                break;
            case 2 :
                BST::erase(x);
                break;
            case 3 :
                printf("%d\n", BST::getRank(x));
                break;
            case 4 :
                printf("%d\n", BST::getNum(x));
                break;
            case 5 :
                printf("%d\n", BST::getPre(x));
                break;
            case 6 :
                printf("%d\n", BST::getNext(x));
                break;
        }
	}
	return 0;
}
