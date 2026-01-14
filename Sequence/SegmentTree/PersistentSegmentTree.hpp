#pragma once
#include <bits/stdc++.h>
#include "kusu_lib/concepts.hpp"
namespace kusu_lib {
template<class S, auto op, auto e>
requires Monoid<S, op, e>
struct PersistentSegmentTree {
  public:
    PersistentSegmentTree() : PersistentSegmentTree(0) {}
    PersistentSegmentTree(int N) : PersistentSegmentTree(std::vector<S>(N, e())) {}
    PersistentSegmentTree(const std::vector<S>& v) : n((int)v.size()) {
        int log = 0;
        {
            int a = 1;
            while(a < n) {
                a *= 2;
                log++;
            }
        }
        size = 1 << log;
        height = log;
        d = std::vector<S>(size * 2, e());
        child.resize(size * 2, {-1, -1});
        for(int i = 1; i < size; i++) child[i] = {i * 2, i * 2 + 1};
        for(int i = 0; i < n; i++) d[i + size] = v[i];
        for(int i = size - 1; i >= 1; i--) d[i] = update(i);
        roots.push_back(1);
    }

    S get(int p) { return get(roots.size() - 1, p); }

    S get(int t, int p) {
        assert(0 <= t && t < (int)roots.size() && 0 <= p && p < n);
        int pos = roots[t];
        int h = height - 1;
        while(h >= 0) {
            if(p >> h & 1) pos = child[pos].second;
            else pos = child[pos].first;
            h--;
        }
        return d[pos];
    }

    void set(int p, S x) { set(roots.size() - 1, p, x); }

    void set(int t, int p, S x) {
        assert(0 <= t && t < (int)roots.size() && 0 <= p && p < n);
        std::function<void(int, int)> dfs = [&](int pos, int h) {
            if(h == -1) {
                child.emplace_back(-1, -1);
                d.push_back(x);
                return;
            }
            if(p >> h & 1) {
                dfs(child[pos].second, h - 1);
                child.emplace_back(child[pos].first, d.size() - 1);
                d.push_back(update(d.size()));
            }
            else {
                dfs(child[pos].first, h - 1);
                child.emplace_back(d.size() - 1, child[pos].second);
                d.push_back(update(d.size()));
            }
        };
        dfs(roots[t], height - 1);
        roots.push_back(d.size() - 1);
    }

    S prod(int l, int r) { return prod(roots.size() - 1, l, r); }

    S prod(int t, int l, int r) {
        assert(0 <= l && l <= r && r <= n && 0 <= t && t < (int)roots.size());
        if(l == r) return e();
        if(l + 1 == r) return get(t, l);
        if(r == size) return op(prod(t, l, r - 1), get(t, r - 1));
        int lpos = roots[t];
        int rpos = roots[t];
        int h = height - 1;
        S lsum = e();
        S rsum = e();
        while((l >> h & 1) == (r >> h & 1)) {
            if(l >> h & 1) {
                lpos = child[lpos].second;
                rpos = child[rpos].second;
            }
            else {
                lpos = child[lpos].first;
                rpos = child[rpos].first;
            }
            h--;
        }
        int lh = h - 1;
        int rh = h - 1;
        lpos = child[lpos].first;
        rpos = child[rpos].second;
        while(true) {
            if((l & -l) >= 1 << (lh + 1) || l == 0) {
                lsum = op(d[lpos], lsum);
                break;
            }
            if(l >> lh & 1) { lpos = child[lpos].second; }
            else {
                lsum = op(d[child[lpos].second], lsum);
                lpos = child[lpos].first;
            }
            lh--;
        }
        while(true) {
            if((r & -r) >= 1 << (rh + 1)) { break; }
            if(r >> rh & 1) {
                rsum = op(rsum, d[child[rpos].first]);
                rpos = child[rpos].second;
            }
            else { rpos = child[rpos].first; }
            rh--;
        }
        return op(lsum, rsum);
    }

  private:
    int n;
    int size;
    int height;
    std::vector<S> d;
    std::vector<std::pair<int, int>> child;
    std::vector<int> roots;
    S update(int i) { return op(d[child[i].first], d[child[i].second]); }
};
}//namespace kusu_lib