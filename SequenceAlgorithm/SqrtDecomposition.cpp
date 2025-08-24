#include <bits/stdc++.h>
template<class S, S (*op)(S, S), S (*e)()> struct SqrtDecomposition {
  public:
    SqrtDecomposition() : SqrtDecomposition(1) {}

    SqrtDecomposition(int N) : SqrtDecomposition(std::vector<S>(N, e())) {}

    SqrtDecomposition(int N, int width) : SqrtDecomposition(std::vector<S>(N, e()), width) {}

    SqrtDecomposition(const std::vector<S>& V)
        : SqrtDecomposition(V, [&]() {
              int width = 0;
              while(width * width < V.size()) width++;
              return width;
          }()) {}
    SqrtDecomposition(const std::vector<S>& V, int width) : v(V), w(width) {
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

    // O(W);
    void set(int p, S x) {
        assert(0 <= p && p < n);
        v[p] = x;
        int k = p / w;
        d[k] = e();
        for(int j = k * w; j < k * w + w; j++) d[k] = op(d[k], v[j]);
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