#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>


inline int read() {
    int x = 0;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    return x;
}


const int MAXN = 500007;


typedef std::pair<int, int> PII;


namespace SegmentTree {
    struct Node {
        int lc, rc, cnt, num;
    } node[MAXN * 2]; 
    int cntNode, root;
#define L(x) node[x].lc
#define R(x) node[x].rc
#define V(x) node[x].num
#define C(x) node[x].cnt
    inline void pushUP(const int &x) {
        if (V(L(x)) == V(R(x))) {
            C(x) = C(L(x)) + C(R(x));
            V(x) = V(L(x));
        }
        else {
            if (C(L(x)) == C(R(x))) {
                C(x) = V(x) = 0;
            }
            else if (C(L(x)) < C(R(x))) {
                C(x) = C(R(x)) - C(L(x));
                V(x) = V(R(x));
            }
            else {
                C(x) = C(L(x)) - C(R(x));	
                V(x) = V(L(x));
            }
        }
        return;
    }
    void modify(int &x, int l, int r, int pos, int val) {
        if (x == 0) x = ++cntNode;
        if (l == r) {
            C(x) = 1;
            V(x) = val;
            return;	
        }
        int mid = (l + r) >> 1;
        if (pos <= mid) modify(L(x), l, mid, pos, val);
        else modify(R(x), mid + 1, r, pos, val);
        pushUP(x);
        return;
    }
    PII query(int cur, int l, int r, int x, int y) {
        if (x <= l && r <= y) {
            return PII(V(cur), C(cur));
        }
        int mid = (l + r) >> 1;
        PII L_ans = PII(0, 0), R_ans = PII(0, 0), ret = PII(0, 0);
        if (x <= mid) {
            L_ans = query(L(cur), l, mid, x, y);
        }
        if (y > mid) {
            R_ans = query(R(cur), mid + 1, r, x, y);	
        }
        
        if (L_ans.first == R_ans.first) {
            ret = PII(L_ans.first, L_ans.second + R_ans.second);
        }
        else {
            if (L_ans.second == R_ans.second) {
                ret = PII(0, 0);
            }	
            else if (L_ans.second < R_ans.second) {
                ret = PII(R_ans.first, R_ans.second - L_ans.second);	
            }
            else {
                ret = PII(L_ans.first, L_ans.second - R_ans.second);
            }
        }
        
        return ret;
    }
#undef L
#undef R
#undef V
#undef C
}

namespace Treap {
    const int INF = 100000000;
    
    struct Node {
        int lc, rc, num, sum, dat;
    } node[MAXN * 10];
    int cntNode,  root[MAXN];

#define L(x) node[x].lc
#define R(x) node[x].rc
#define V(x) node[x].num
#define S(x) node[x].sum
#define D(x) node[x].dat

    inline int newNode(int val) {
        ++cntNode;
        V(cntNode) = val;
        D(cntNode) = std::rand();
        S(cntNode) = 1;
        return cntNode;
    }
    inline void pushUP(int x) {
        if (x != 0) {
            S(x) = S(L(x)) + S(R(x)) + 1;
        }
        return;
    }
    inline void build(int n) {
        for (int i = 1; i <= n; ++i) {
            root[i] = newNode(-INF);
            R(root[i]) = newNode(INF);
            pushUP(root[i]);
        }
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
        /* no use
        if (val == V(p)) {
            ++C(p);
            pushUP(p);
            return;	
        }
        */
        
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
            /* no use
            if (C(p) > 1) {
                --C(p);	
            }
            */	
            
            if (L(p) || R(p)) {
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
        return S(L(p)) + 1 + getRank(R(p), val);
    }
#undef L
#undef R
#undef V
#undef S
#undef D
}

namespace BST {
    inline void init(int n) {
        Treap::build(n);
        return;
    }
    inline bool query(int val, int l, int r) {
        return (Treap::getRank(Treap::root[val], r) - Treap::getRank(Treap::root[val], l - 1) > ((r - l + 1) >> 1));
    }	
    inline void insert(int val, int pos) {
        Treap::insert(Treap::root[val], pos);
        return;	
    }
    inline void erase (int val, int pos) {
        Treap::erase(Treap::root[val], pos);
        return;
    }
}


int org_data[MAXN];


inline void data_init(int n) {
    BST::init(n);
    for (int i = 1; i <= n; ++i) {
        org_data[i] = read();
        SegmentTree::modify(SegmentTree::root, 1, n, i, org_data[i]);
        BST::insert(org_data[i], i);
    }
    return;
}


inline void solve(int n, int m) {
    for (int i = 1; i <= m; ++i) {
        int l = read();
        int r = read();
        int s = read();
        int k = read();
        PII ans = SegmentTree::query(SegmentTree::root, 1, n, l, r);
        if (ans.first != 0) {
            if (BST::query(ans.first, l,  r) == true) {
                s = ans.first;	
            }
        }
        printf("%d\n", s);
        for (int i = 1; i <= k; ++i) {
            int pos = read();
            BST::erase(org_data[pos], pos);
            BST::insert(s, pos);
            SegmentTree::modify(SegmentTree::root, 1, n, pos, s);
            org_data[pos] = s;
        }
    }
    PII ans = SegmentTree::query(SegmentTree::root, 1, n, 1, n);
    if (ans.first != 0 && BST::query(ans.first, 1, n) ==  true) {
        printf("%d\n", ans.first);
    }
    else {
        printf("-1\n");
    }
}


int main() {
    srand(time(NULL));
    int n = read();
    int m = read();
    data_init(n);
    solve(n, m);
    return 0;
}
