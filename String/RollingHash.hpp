#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
class RollingHash {
    template<class S, S (*op)(S, S), S (*e)()> struct SegTree {
      public:
        SegTree() : SegTree(0) {}
        SegTree(int n) : SegTree(std::vector<S>(n, e())) {}
        SegTree(const std::vector<S>& v) : _n(int(v.size())) {
            log = 0;
            while((1 << log) < _n) log++;
            size = 1 << log;
            d = std::vector<S>(2 * size, e());
            for(int i = 0; i < _n; i++) d[size + i] = v[i];
            for(int i = size - 1; i >= 1; i--) { update(i); }
        }

        void set(int p, S x) {
            assert(0 <= p && p < _n);
            p += size;
            d[p] = x;
            for(int i = 1; i <= log; i++) update(p >> i);
        }

        S get(int p) {
            assert(0 <= p && p < _n);
            return d[p + size];
        }

        S prod(int l, int r) {
            assert(0 <= l && l <= r && r <= _n);
            S sml = e(), smr = e();
            l += size;
            r += size;

            while(l < r) {
                if(l & 1) sml = op(sml, d[l++]);
                if(r & 1) smr = op(d[--r], smr);
                l >>= 1;
                r >>= 1;
            }
            return op(sml, smr);
        }

        S all_prod() { return d[1]; }

        template<bool (*f)(S)> int max_right(int l) {
            return max_right(l, [](S x) { return f(x); });
        }
        template<class F> int max_right(int l, F f) {
            assert(0 <= l && l <= _n);
            assert(f(e()));
            if(l == _n) return _n;
            l += size;
            S sm = e();
            do {
                while(l % 2 == 0) l >>= 1;
                if(!f(op(sm, d[l]))) {
                    while(l < size) {
                        l = (2 * l);
                        if(f(op(sm, d[l]))) {
                            sm = op(sm, d[l]);
                            l++;
                        }
                    }
                    return l - size;
                }
                sm = op(sm, d[l]);
                l++;
            } while((l & -l) != l);
            return _n;
        }

        template<bool (*f)(S)> int min_left(int r) {
            return min_left(r, [](S x) { return f(x); });
        }
        template<class F> int min_left(int r, F f) {
            assert(0 <= r && r <= _n);
            assert(f(e()));
            if(r == 0) return 0;
            r += size;
            S sm = e();
            do {
                r--;
                while(r > 1 && (r % 2)) r >>= 1;
                if(!f(op(d[r], sm))) {
                    while(r < size) {
                        r = (2 * r + 1);
                        if(f(op(d[r], sm))) {
                            sm = op(d[r], sm);
                            r--;
                        }
                    }
                    return r + 1 - size;
                }
                sm = op(d[r], sm);
            } while((r & -r) != r);
            return 0;
        }

      private:
        int _n, size, log;
        std::vector<S> d;

        void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
    };

    using ull = unsigned long long;
    static const ull MASK30 = (1ULL << 30) - 1;
    static const ull MASK31 = (1ULL << 31) - 1;
    static const ull MOD = (1ULL << 61) - 1;
    static const ull POSITIVIZER = MOD * 4;
    static const ull MASK61 = MOD;
    static ull base;
    static std::vector<ull> powmemo;

    int n;
    std::vector<ull> V;
    std::vector<ull> hash;

    static std::pair<ull, ull> op(std::pair<ull, ull> a, std::pair<ull, ull> b) {
        return {CalcMod(a.first + Mul(b.first, powmemo[a.second])), a.second + b.second};
    }
    static std::pair<ull, ull> e() { return {0, 0}; }
    SegTree<std::pair<ull, ull>, op, e> seg;
    bool made_seg = false;

    void set_seg() {
        if(made_seg) return;
        made_seg = true;
        std::vector<std::pair<ull, ull>> v(n);
        for(int i = 0; i < n; i++) v[i] = {V[i], 1};
        seg = SegTree<std::pair<ull, ull>, op, e>(v);
    }

    // a*b mod 2^61-1
    static ull Mul(ull a, ull b) {
        ull au = a >> 31;
        ull ad = a & MASK31;
        ull bu = b >> 31;
        ull bd = b & MASK31;
        ull mid = ad * bu + au * bd;
        ull midu = mid >> 30;
        ull midd = mid & MASK30;
        return CalcMod(au * bu * 2 + midu + (midd << 31) + ad * bd);
    }

    // mod 2^61-1
    static ull CalcMod(ull x) {
        ull xu = x >> 61;
        ull xd = x & MASK61;
        ull res = xu + xd;
        if(res >= MOD) res -= MOD;
        return res;
    }

    static void set_base() {
        if(base == 0) {
            std::mt19937 rng(time(NULL));
            base = rng();
        }
    }

  public:
    RollingHash(const std::vector<ull>& v) : n(v.size()), V(v) {
        set_base();
        hash.resize(n + 1);
        for(int i = 0; i < n; i++) hash[i + 1] = CalcMod(Mul(hash[i], base) + V[i]);
        if((int)powmemo.size() < n + 1) {
            int m = powmemo.size();
            powmemo.resize(n + 1);
            powmemo[0] = 1;
            for(int i = std::max(0, m - 1); i < n; i++)
                powmemo[i + 1] = CalcMod(Mul(powmemo[i], base));
        }
    }

    RollingHash(const std::string& S) : RollingHash(std::vector<ull>(S.begin(), S.end())) {}

    //check x is proper
    void set(int i, ull x) {
        if(!made_seg) set_seg();
        seg.set(i, {x, 1});
    }
    //begin <= x < end
    ull GetHash(int begin, int end) {
        if(made_seg) return seg.prod(begin, end).first;
        return CalcMod(hash[end] + POSITIVIZER - Mul(hash[begin], powmemo[end - begin]));
    }
    int size() { return n; }
    ull get_base_pow(int n) { return powmemo[n]; }
    ull GetSectionsHash(std::vector<std::pair<int, int>> S) {
        ull res = 0;
        for(auto [l, r]: S) {
            res = Mul(res, powmemo[r - l]);
            res = CalcMod(res + GetHash(l, r));
        }
        return res;
    }

    static ull CalcHash(const std::vector<ull>& S) {
        ull res = 0;
        for(int i = 0; i < (int)S.size(); i++) {
            res = Mul(res, base);
            res += S[i];
            res = CalcMod(res);
        }
        return res;
    }

    static ull CalcHash(const std::string& S) {
        return CalcHash(std::vector<ull>(S.begin(), S.end()));
    }
};
unsigned long long RollingHash::base = 0;
std::vector<unsigned long long> RollingHash::powmemo;
}//namespace kusu_lib