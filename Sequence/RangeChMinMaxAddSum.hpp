#pragma once
#include <bits/stdc++.h>
#include "SegmentTree/SegmentTreeBeats.hpp"
namespace kusu_lib {

template<class T>
struct RangeChMinMaxAddSumFunc {
    static constexpr T INF = std::numeric_limits<T>::max() / 4;

    //a1 < a2, b1 < b2
    static T second_smallest(T a1, T a2, T b1, T b2) {
        if(a2 < b1) return a2;
        if(b2 < a1) return b2;
        if(a1 == b1) return std::min(a2, b2);
        return std::max(a1, b1);
    }

    //a1 < a2, b1 < b2
    static T second_largest(T a1, T a2, T b1, T b2) {
        if(a1 > b2) return a1;
        if(b1 > a2) return b1;
        if(a2 == b2) return std::max(a1, b1);
        return std::min(a2, b2);
    }

    struct S {
        T s = INF, l = -INF, ss = INF, sl = -INF,
          sum = 0;//smallest, largest, second smallest, second largest, sum
        int len = 0, sc = 0, lc = 0;//length, number of smallest, nubmer of largest
        bool fail = false;
        S() {}
        S(T x, int sz) : s(x), l(x), ss(INF), sl(-INF), sum(x * sz), len(sz), sc(sz), lc(sz) {}
    };

    static S op(S a, S b) {
        S res;
        res.s = std::min(a.s, b.s);
        res.l = std::max(a.l, b.l);
        res.ss = second_smallest(a.s, a.ss, b.s, b.ss);
        res.sl = second_largest(a.sl, a.l, b.sl, b.l);
        res.sum = a.sum + b.sum;
        res.len = a.len + b.len;
        res.sc = a.sc * (a.s == res.s) + b.sc * (b.s == res.s);
        res.lc = a.lc * (a.l == res.l) + b.lc * (b.l == res.l);
        return res;
    }

    static S e() { return S(); }

    struct F {
        T low = -INF, high = INF, add = 0;
    };

    static S mapping(F f, S x) {
        if(x.len == 0) return e();
        if(x.s == x.l || f.low == f.high || x.l <= f.low || f.high <= x.s) {
            return S(std::clamp(x.s, f.low, f.high) + f.add, x.len);
        }
        if(x.ss == x.l) {
            x.s = x.sl = std::max(x.s, f.low) + f.add;
            x.l = x.ss = std::min(x.l, f.high) + f.add;
            x.sum = x.s * x.sc + x.l * x.lc;
            return x;
        }
        if(f.low < x.ss && x.sl < f.high) {
            T ns = std::max(x.s, f.low);
            T nl = std::min(x.l, f.high);
            x.sum += (ns - x.s) * x.sc + (nl - x.l) * x.lc + f.add * x.len;
            x.s = ns + f.add;
            x.l = nl + f.add;
            x.ss += f.add;
            x.sl += f.add;
            return x;
        }
        x.fail = true;
        return x;
    }

    static F composition(F f, F g) {
        F res;
        res.low = std::clamp(g.low + g.add, f.low, f.high) - g.add;
        res.high = std::clamp(g.high + g.add, f.low, f.high) - g.add;
        res.add = g.add + f.add;
        return res;
    }

    static F id() { return F(); }
};

template<class T>
struct RangeChMinMaxAddSum
    : SegmentTreeBeats<typename RangeChMinMaxAddSumFunc<T>::S, RangeChMinMaxAddSumFunc<T>::op,
                       RangeChMinMaxAddSumFunc<T>::e, typename RangeChMinMaxAddSumFunc<T>::F,
                       RangeChMinMaxAddSumFunc<T>::mapping, RangeChMinMaxAddSumFunc<T>::composition,
                       RangeChMinMaxAddSumFunc<T>::id> {
  private:
    using Base =
        SegmentTreeBeats<typename RangeChMinMaxAddSumFunc<T>::S, RangeChMinMaxAddSumFunc<T>::op,
                         RangeChMinMaxAddSumFunc<T>::e, typename RangeChMinMaxAddSumFunc<T>::F,
                         RangeChMinMaxAddSumFunc<T>::mapping,
                         RangeChMinMaxAddSumFunc<T>::composition, RangeChMinMaxAddSumFunc<T>::id>;
    using Base::Base;
    using S = typename RangeChMinMaxAddSumFunc<T>::S;
    using F = typename RangeChMinMaxAddSumFunc<T>::F;
    static constexpr T INF = RangeChMinMaxAddSumFunc<T>::INF;

    static std::vector<S> make_vector(const std::vector<T>& v) {
        std::vector<S> V(v.size());
        for(int i = 0; i < (int)v.size(); i++) { V[i] = S(v[i], 1); }
        return V;
    }

  public:
    RangeChMinMaxAddSum(const std::vector<T>& v) : Base(make_vector(v)) {}

    void chmin(int l, int r, T a) { this->apply(l, r, F{-INF, a, 0}); }

    void chmax(int l, int r, T a) { this->apply(l, r, F{a, INF, 0}); }

    void clamp(int l, int r, T a, T b) {
        assert(a <= b);
        this->apply(l, r, F{a, b, 0});
    }

    void assign(int l, int r, T a) { this->apply(l, r, F{a, a, 0}); }

    void add(int l, int r, T a) { this->apply(l, r, F{-INF, INF, a}); }

    void set(int p, T x) { this->set(p, S(x, 1)); }

    T val(int p) { return this->get(p).s; }

    T sum(int l, int r) { return this->prod(l, r).sum; }

    T max(int l, int r) { return this->prod(l, r).l; }

    T min(int l, int r) { return this->prod(l, r).s; }

    T second_max(int l, int r) { return this->prod(l, r).sl; }

    T second_min(int l, int r) { return this->prod(l, r).ss; }

    T max_count(int l, int r) { return this->prod(l, r).lc; }

    T min_count(int l, int r) { return this->prod(l, r).sc; }
};
}//namespace kusu_lib