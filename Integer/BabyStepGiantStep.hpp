#pragma once
#include "../Other/custom_hash.hpp"
#include <bits/stdc++.h>
namespace kusu_lib {
//return min n s.t. f^n(x) = y & n <= M. if such n does not exist, return -1.
//this may return n s.t. f^n(x) = y & M < n.
//m should be sqrt(M)
template<class F>
long long BabyStepGiantStep(long long x, long long y, long long M, long long m, F F_INV, F F_M) {
    std::unordered_set<long long, kusu_lib::custom_hash> hash;
    long long k = y;
    long long fmx = x;
    for(long long i = 0; i < m; i++) {
        hash.insert(k);
        k = F_INV(k);
    }
    for(long long i = 0; i <= M / m; i++) {
        if(hash.count(fmx)) {
            long long v = F_M(fmx);
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