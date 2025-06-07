#include <bits/stdc++.h>
template<class S, S (*op)(S, S), S (*e)()> struct DisjointSparseTable {
  private:
    int N;
    int h;
    S *T;
    std::vector<S> _v;

  public:
    DisjointSparseTable() : N(0), h(0) {}
    DisjointSparseTable(const std::vector<S> &v) : _v(v) {
        if(v.empty()) {
            DisjointSparseTable();
            return;
        }
        N = v.size();
        h = 31 - std::__countl_zero(N - 1);
        T = new S[N * h];
        for(int k = 0; k < h; k++) {
            int w = 1 << k;
            int base = N * k;
            for(int i = w; i < N; i += 2 * w) {
                T[base + i - 1] = v[i - 1];
                for(int j = base + i - 2; j >= base + i - w; j--) T[j] = op(v[j - base], T[j + 1]);
                T[base + i] = v[i];
                for(int j = base + i + 1; j < std::min(base + i + w, N * (k + 1)); j++) {
                    T[j] = op(T[j - 1], v[j - base]);
                }
            }
        }
    }

    S get(int p){
        assert(0<=p&&p<N);
        return _v[p];
    }

    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= N);
        if(l==r) return e();
        if(l+1==r) return _v[l];
        r--;
        int p = 30 - std::__countl_zero(l ^ r);
        return op(T[p * N + l], T[p * N + r]);
    }

    int size(){ return N;}
};