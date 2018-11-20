#include <iostream>
#include <algorithm>
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


const int MAXN = 50007;


int que_1[MAXN], que_2[MAXN], tmp[MAXN << 1];


namespace SegmentTree {
	#define C(x) node[x].cnt
	#define L(x) node[x].lc
	#define R(x) node[x].rc
	struct Node {
		int lc, rc, cnt;	
	} node[MAXN << 7];
	int root[MAXN], cntNode;
	inline int newNode() {
		Node *it = &node[++cntNode];	
		it->lc = it->rc = it->cnt = 0;
		return cntNode;
	}
	void modify(int &cur, int l, int r, int aim, int delta) {
		if (cur == 0) cur = newNode();
		C(cur) += delta;
		if (l == r) return;
		int mid = (l + r) >> 1;
		if (aim <= mid) {
			modify(L(cur), l, mid, aim, delta);
		}
		else {
			modify(R(cur), mid + 1, r, aim, delta);	
		}
		return;
	}
	int queryRank(int cur, int l, int r, int aim) {
		if (cur == 0) return 0;
		if (l == r) return C(cur);
		int mid = (l + r) >> 1;
		if (aim <= mid) return queryRank(L(cur), l, mid, aim);
		else return C(L(cur)) + queryRank(R(cur), mid + 1, r, aim);
	}
	int queryKth(int tot_1, int tot_2, int l, int r, int k) {
		if (l == r) return l;
		int cnt = 0;
		for (int i = 1; i <= tot_1; ++i) {
			cnt += C(L(que_1[i]));	
		}
		for (int i = 1; i <= tot_2; ++i) {
			cnt -= C(L(que_2[i]));
		}
		int mid = (l + r) >> 1;
		if (cnt >= k) {
			for (int i = 1; i <= tot_1; ++i) {
				que_1[i] = L(que_1[i]);
			}
			for (int i = 1; i <= tot_2; ++i) {
				que_2[i] = L(que_2[i]);
			}
			return queryKth(tot_1, tot_2, l, mid, k);
		}
		else {
			for (int i = 1; i <= tot_1; ++i) {
				que_1[i] = R(que_1[i]);
			}
			for (int i = 1; i <= tot_2; ++i) {
				que_2[i] = R(que_2[i]);
			}	
			return queryKth(tot_1, tot_2, mid + 1, r, k - cnt);
		}
	}
}


namespace BIT {
	inline int lowbit(int x) {
		return x & (-x);
	}
	inline void modify(int pos, int n, int tot, int val, int delta) {
		for (int i = pos; i <= n; i += lowbit(i)) {
			SegmentTree::modify(SegmentTree::root[i], 1, tot, val, delta);
		}
		return;
	}
	inline int getRank(int l, int r, int val, int tot) {
		--l;
		int ret = 0;
		for (int i = r; i > 0; i -= lowbit(i)) {
			ret += SegmentTree::queryRank(SegmentTree::root[i], 1, tot, val);
		}
		for (int i = l; i > 0; i -= lowbit(i)) {
			ret -= SegmentTree::queryRank(SegmentTree::root[i], 1, tot, val);
		}
		return ret;
	}
	inline int getKth(int l, int r, int k, int tot) {
		--l;
		int tot_1 = 0, tot_2 = 0;
		for (int i = r; i > 0; i -= lowbit(i)) {
			que_1[++tot_1] = SegmentTree::root[i];
		}
		for (int i = l; i > 0; i -= lowbit(i)) {
			que_2[++tot_2] = SegmentTree::root[i];
		}
		return tmp[SegmentTree::queryKth(tot_1, tot_2, 1, tot, k)];
	}
}


int data[MAXN];


struct Option {
	int type, l, r, k;
} option[MAXN];


int main() {
	int n = read();
	int m = read();
	int tot = 0;
	for (int i = 1; i <= n; ++i) {
		data[i] = read();
		tmp[++tot] = data[i];
	}
	for (int i = 1; i <= m; ++i) {
		option[i].type = read();
		if (option[i].type == 3) {
			option[i].l = read();
			option[i].k = read();	
		}
		else {
			option[i].l = read();
			option[i].r = read();
			option[i].k = read();
		}
		
		if (option[i].type != 2) {
			tmp[++tot] = option[i].k;
		}
	}	
	
	std::sort(tmp + 1, tmp + 1 + tot);
	tot = std::unique(tmp + 1, tmp + 1 + tot) - tmp - 1;
	
	for (int i = 1; i <= n; ++i) {
		data[i] = std::lower_bound(tmp + 1, tmp + 1 + tot, data[i]) - tmp;
		BIT::modify(i, n, tot, data[i], 1);
	}
	
	for (int i = 1; i <= m; ++i) {
		switch (option[i].type) {
			case 1 : {
				option[i].k = std::lower_bound(tmp + 1, tmp + 1 + tot, option[i].k) - tmp;
				printf("%d\n", BIT::getRank(option[i].l, option[i].r, option[i].k - 1, tot) + 1);
				break;
			}
			case 2 : {
				printf("%d\n", BIT::getKth(option[i].l, option[i].r, option[i].k, tot));
				break;
			}
			case 3 : {
				BIT::modify(option[i].l, n, tot, data[option[i].l], -1);	
				data[option[i].l] = std::lower_bound(tmp + 1, tmp + 1 + tot, option[i].k) - tmp;
				BIT::modify(option[i].l, n, tot, data[option[i].l], 1);	
				break;
			}
			case 4 : {
				option[i].k = std::lower_bound(tmp + 1, tmp + 1 + tot, option[i].k) - tmp;
				int rank = BIT::getRank(option[i].l, option[i].r, option[i].k - 1, tot);
				if (rank == 0) printf("-2147483647\n");
				else printf("%d\n", BIT::getKth(option[i].l, option[i].r, rank, tot));
				break;
			}
			case 5 : {
				option[i].k = std::lower_bound(tmp + 1, tmp + 1 + tot, option[i].k) - tmp;
				int rank = BIT::getRank(option[i].l, option[i].r, option[i].k, tot);
				if (rank == option[i].r - option[i].l + 1) printf("2147483647\n");
				else printf("%d\n", BIT::getKth(option[i].l, option[i].r, rank + 1, tot));
			}
		}
	}
	return 0;
}
