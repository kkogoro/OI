#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstdlib>


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


const int MAXN = 500007;
const int INF = 500000007;

struct Treap {
    struct Node {
        int lc, rc, num, cnt, sum, dat;    
    } node[MAXN << 2];
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
    int lower(int val) {
        int ans = 0, p = root;
        while (p > 0) {
            if (val > V(p)) {
                ans = p;
                p = R(p);    
            }
            else {
            	if (val == V(p)) {
                    ans = p;
                    break;
                }
                p = L(p);
            }
        }
        return V(ans);
    }
    int upper(int val) {
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
} all, delta;


int head[MAXN], tail[MAXN];


inline int ABS(int x) {
    return x < 0 ? -x : x;
}


int main() {
    srand(time(NULL));
    int n = read();
    int m = read();
    all.build(), delta.build();
    int MIN_GAP = INF, MIN_SORT_GAP = INF;
    for (int i = 1; i <= n; ++i) {
        head[i] = read();
        tail[i] = INF;
        if (i != 1) {
            MIN_GAP = std::min(MIN_GAP, ABS(head[i] - head[i - 1]));
            MIN_SORT_GAP = std::min(MIN_SORT_GAP, std::min(ABS(head[i] - all.lower(head[i])), ABS(head[i] - all.upper(head[i])) ) );
            delta.insert(delta.root, ABS(head[i] - head[i - 1]));
        }
        all.insert(all.root, head[i]);
    }
    for (int i = 1; i <= m; ++i) {
        char opt[14];
        scanf("%s", opt);
        switch (opt[4]) {
            case 'R' : {
                int x = read();
                int y = read();
                if (tail[x] == INF) {
                    delta.insert(delta.root, ABS(y - head[x]));
                    if (x != n) {
                        delta.erase(delta.root, ABS(head[x + 1] - head[x]));
                        delta.insert(delta.root, ABS(head[x + 1] - y));
                    }
                }
                else {
                    delta.insert(delta.root, ABS(y - tail[x]));
                    if (x != n) {
                        delta.erase(delta.root, ABS(head[x + 1] - tail[x]));
                        delta.insert(delta.root, ABS(head[x + 1] - y));
                    }
                }
                MIN_GAP = delta.upper(-INF);
                MIN_SORT_GAP = std::min(MIN_SORT_GAP, std::min(ABS(y - all.lower(y)), ABS(y - all.upper(y)) ) );
                all.insert(all.root, y);
                tail[x] = y;
                break;
            }
            case 'G' : {
                printf("%d\n", MIN_GAP);
                break;
            }
            case 'S' : {
                printf("%d\n", MIN_SORT_GAP);
                break;
            }
        }
    }
    return 0;
}
