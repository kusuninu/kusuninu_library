#pragma once
#include "../Other/custom_hash.hpp"
#include <bits/stdc++.h>
namespace kusu_lib {
//return min n s.t. f^n(x) = y & n <= M. if such n does not exist, return -1.
//this may return n s.t. f^n(x) = y & M < n.
//m should be sqrt(M)
template<class T = long long, class Hash = kusu_lib::custom_hash, class F>
requires requires(F f, T x, Hash h) {
    { f(x) } -> std::same_as<T>;
    { h(x) } -> std::same_as<size_t>;
}
long long BabyStepGiantStep(T x, T y, long long M, long long m, F F_INV, F F_M) {
    std::unordered_set<T, Hash> hash;
    T k = y;
    T fmx = x;
    for(long long i = 0; i < m; i++) {
        hash.insert(k);
        k = F_INV(k);
    }
    for(long long i = 0; i <= M / m; i++) {
        if(hash.count(fmx)) {
            T v = F_M(fmx);
            long long res = -1;
            for(long long j = m - 1; j >= 0; j--) {
                v = F_INV(v);
                if(v == y) res = i * m + j;
            }
            assert(res != -1);
            return res;
        }
        fmx = F_M(fmx);
    }
    return -1;
}
}//namespace kusu_lib