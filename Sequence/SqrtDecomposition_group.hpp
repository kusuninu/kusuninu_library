#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
//inv(op(x,y),y) = x
template<class S, S (*op)(S, S), S (*e)()> struct SqrtDecomposition_group {
  public:
    SqrtDecomposition_group() : SqrtDecomposition_group(1) {}

    SqrtDecomposition_group(int N) : SqrtDecomposition_group(std::vector<S>(N, e())) {}

    SqrtDecomposition_group(int N, int width)
        : SqrtDecomposition_group(std::vector<S>(N, e()), width) {}

    SqrtDecomposition_group(const std::vector<S>& V)
        : SqrtDecomposition_group(V, [&]() {
              int width = 0;
              while(width * width < V.size()) width++;
              return width;
          }()) {}
    SqrtDecomposition_group(const std::vector<S>& V, int width) : v(V), w(width) {
        assert(0 < w);
        n = v.size();
        s = (n - 1) / w + 1;
        d.resize(s, e());
        size = w * s;
        v.resize(size, e());
        for(int i = 0; i < s; i++) {
            for(int j = i * w; j < i * w + w; j++) d[i] = op(d[i], v[j]);
        }
    }

    // O(1);
    void set(int p, S x, S inv) {
        assert(0 <= p && p < n);
        int k = p / w;
        d[k] = op(d[k], inv);
        d[k] = op(d[k], x);
        v[p] = x;
    }

    // O(1)
    S get(int p) {
        assert(0 <= p && p < n);
        return v[p];
    }

    // O(N/W+W)
    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= n);
        int a = (l == 0 ? 0 : (l - 1) / w * w + w);
        int b = r / w * w;
        S res = e();
        for(int i = l; i < a; i++) res = op(res, v[i]);
        for(int i = a / w; i < b / w; i++) res = op(res, d[i]);
        for(int i = b; i < r; i++) res = op(res, v[i]);
        return res;
    }

  private:
    int n;
    int w;
    int s;
    int size;
    std::vector<S> v;
    std::vector<S> d;
};
}//namespace kusu_lib