#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<class S, class F, S (*mapping)(F, S), F (*composition)(F, F), F (*id)()>
struct DualSegmentTree {
  public:
    DualSegmentTree() : _n(0) {}
    DualSegmentTree(const std::vector<S>& v) : _n(int(v.size())) {
        if(_n == 0) return;
        log = 31 - std::__countl_zero(_n - 1);
        size = 1 << log;
        d = v;
        lz = std::vector<F>(size * 2, id());
    }

    void set(int p, S x) {
        assert(0 <= p && p < _n);
        d[p] = x;
        p += size;
        for(int i = log; i >= 1; i--) push(p >> i);
        lz[p] = id();
    }

    S get(int p) {
        assert(0 <= p && p < _n);
        S res = d[p];
        p += size;
        for(int i = 0; i <= log; i++) res = mapping(lz[p >> i], res);
        return res;
    }

    void apply(int p, F f) {
        assert(0 <= p && p < _n);
        p += size;
        for(int i = log; i >= 1; i--) push(p >> i);
        lz[p] = composition(f, lz[p]);
    }

    void apply(int l, int r, F f) {
        assert(0 <= l && l <= r && r <= _n);
        if(l == r) return;
        l += size;
        r += size;
        for(int i = log; i >= 1; i--) {
            if(((l >> i) << i) != l) push(l >> i);
            if(((r >> i) << i) != r) push((r - 1) >> i);
        }
        while(l < r) {
            if(l & 1) {
                lz[l] = composition(f, lz[l]);
                l++;
            }
            if(r & 1) {
                lz[r - 1] = composition(f, lz[r - 1]);
                r--;
            }
            l >>= 1;
            r >>= 1;
        }
    }

  private:
    int _n, size, log;
    std::vector<S> d;
    std::vector<F> lz;

    void push(int k) {
        lz[2 * k] = composition(lz[k], lz[2 * k]);
        lz[2 * k + 1] = composition(lz[k], lz[2 * k + 1]);
        lz[k] = id();
    }
};
}//namespace kusu_lib