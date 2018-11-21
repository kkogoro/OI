#include <iostream>
#include <cstdio>
#include <set>


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


int head[MAXN], tail[MAXN];


inline int ABS(int x) {
    return x < 0 ? -x : x;
}


std::multiset<int> delta, all;


int main() {
    int n = read();
    int m = read();
    int MIN_GAP = INF, MIN_SORT_GAP = INF;
    for (int i = 1; i <= n; ++i) {
        head[i] = read();
        tail[i] = INF;
        if (i != 1) {
            MIN_GAP = std::min(MIN_GAP, ABS(head[i] - head[i - 1]));
 			std::multiset<int>::iterator iter = all.lower_bound(head[i]);
        	MIN_SORT_GAP = std::min(MIN_SORT_GAP, std::min(ABS(head[i] - *iter), ABS(head[i] - *all.upper_bound(head[i])) ) );
        	if (iter != all.begin()) {
                --iter;
            	MIN_SORT_GAP = std::min(MIN_SORT_GAP, ABS(head[i] - *iter));
            }
            delta.insert(ABS(head[i] - head[i - 1]));
        }
        all.insert(head[i]);
    }
    for (int i = 1; i <= m; ++i) {
        char opt[14];
        scanf("%s", opt);
        switch (opt[4]) {
            case 'R' : {
                int x = read();
                int y = read();
                if (tail[x] == INF) {
                    delta.insert(ABS(y - head[x]));
                    if (x != n) {
                        delta.erase(delta.find(ABS(head[x + 1] - head[x])));
                        delta.insert(ABS(head[x + 1] - y));
                    }
                }
                else {
                    delta.insert(ABS(y - tail[x]));
                    if (x != n) {
                        delta.erase(delta.find(ABS(head[x + 1] - tail[x])));
                        delta.insert(ABS(head[x + 1] - y));
                    }
                }
                MIN_GAP = *delta.begin();
                std::multiset<int>::iterator iter = all.lower_bound(y);
                MIN_SORT_GAP = std::min(MIN_SORT_GAP, std::min(ABS(y - *iter), ABS(y - *all.upper_bound(y)) ) );
                if (iter != all.begin()) {
                	--iter;
            	    MIN_SORT_GAP = std::min(MIN_SORT_GAP, ABS(y - *iter));
            	}
                all.insert(y);
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
