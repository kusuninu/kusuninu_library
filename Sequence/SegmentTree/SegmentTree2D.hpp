#pragma once
#include <bits/stdc++.h>
#include "kusu_lib/concepts.hpp"
namespace kusu_lib {
template<class S, auto op, auto e>
requires Monoid<S, op, e>
struct SegmentTree2D {
  public:
    SegmentTree2D() : H(0), W(0) {}
    SegmentTree2D(int H, int W) : H(H), W(W) {
        assert(1 <= H && 1 <= W);
        h_s = 1 << (32 - std::__countl_zero(H - 1));
        w_s = 1 << (32 - std::__countl_zero(W - 1));
        d = new S[h_s * w_s * 4];
        for(int i = 0; i < h_s * w_s * 4; i++) d[i] = e();
    }

    SegmentTree2D(const std::vector<std::vector<S>>& V) {
        H = V.size();
        assert(1 <= H);
        W = V[0].size();
        assert(1 <= W);
        h_s = 1 << (32 - std::__countl_zero(H - 1));
        w_s = 1 << (32 - std::__countl_zero(W - 1));
        d = new S[h_s * w_s * 4];
        for(int i = 0; i < h_s * w_s * 4; i++) d[i] = e();
        for(int i = 0; i < H; i++) {
            assert((int)V[i].size() == W);
            for(int j = 0; j < W; j++) d[index(i + h_s, j + w_s)] = V[i][j];
        }
        for(int i = h_s; i < h_s * 2; i++) {
            for(int j = w_s - 1; j >= 1; j--) {
                d[index(i, j)] = op(d[index(i, j * 2)], d[index(i, j * 2 + 1)]);
            }
        }
        for(int i = h_s - 1; i >= 1; i--) {
            for(int j = 1; j < w_s * 2; j++) {
                d[index(i, j)] = op(d[index(i * 2, j)], d[index(i * 2 + 1, j)]);
            }
        }
    }

    void set(int i, int j, S x) {
        assert(0 <= i && i < H && 0 <= j && j < W);
        i += h_s;
        j += w_s;
        d[index(i, j)] = x;
        for(int p = j >> 1; p >= 1; p >>= 1) {
            d[index(i, p)] = op(d[index(i, p * 2)], d[index(i, p * 2 + 1)]);
        }
        for(int p = i >> 1; p >= 1; p >>= 1) {
            for(int q = j; q >= 1; q >>= 1) {
                d[index(p, q)] = op(d[index(p * 2, q)], d[index(p * 2 + 1, q)]);
            }
        }
    }

    S get(int i, int j) const {
        assert(0 <= i && i < H && 0 <= j && j < W);
        return d[index(i + h_s, j + w_s)];
    }

    S prod(int l1, int r1, int l2, int r2) const {
        assert(0 <= l1 && l1 <= r1 && r1 <= H && 0 <= l2 && l2 <= r2 && r2 <= W);
        l1 += h_s;
        l2 += h_s;
        r1 += w_s;
        r2 += w_s;
        S res = e();
        while(l1 < r1) {
            if(l1 & 1) {
                w_prod(l1, l2, r2, res);
                l1++;
            }
            if(r1 & 1) {
                r1--;
                w_prod(r1, l2, r2, res);
            }
            l1 >>= 1;
            r1 >>= 1;
        }
        return res;
    }

    S all_prod() const { return d[1][1]; }

  private:
    int H;
    int W;
    S* d;
    int h_s;
    int w_s;
    int index(int h, int w) const { return h * 2 * w_s + w; }
    void w_prod(int i, int l, int r, S& res) const {
        while(l < r) {
            if(l & 1) {
                res = op(res, d[index(i, l)]);
                l++;
            }
            if(r & 1) {
                r--;
                res = op(res, d[index(i, r)]);
            }
            l >>= 1;
            r >>= 1;
        }
    }
};
}//namespace kusu_lib