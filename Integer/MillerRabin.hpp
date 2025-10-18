#pragma once
#include "Montgomery.hpp"
#include <bits/stdc++.h>
namespace kusu_lib {
bool MillerRabinTest(long long N, const std::vector<long long>& A) {
    assert(N % 2 == 1);
    assert(2 < N);
    using mint = Montgomery<unsigned long long>::MontgomeryInt;
    Montgomery<unsigned long long> mt(N);
    long long t = 0;
    long long d = N - 1;
    while(!(d & 1)) {
        d >>= 1;
        t++;
    }
    mint Nminus1(N - 1, &mt);
    for(auto a: A) {
        if(N <= a) return true;
        mint x(a, &mt);
        x = x.pow(d);
        if(x.val() == 1) continue;
        bool br = false;
        for(int i = 0; i < t; i++) {
            if(x == Nminus1) {
                br = true;
                break;
            }
            if(i != t - 1) x *= x;
        }
        if(!br) return false;
    }
    return true;
}

bool is_prime(long long N) {
    if(N <= 1) return false;
    if(N == 2) return true;
    if(N % 2 == 0) return false;
    if(N < 4759123141LL) return MillerRabinTest(N, {2, 7, 61});
    return MillerRabinTest(N, {2, 325, 9375, 28178, 450775, 9780504, 1795265022});
}
}//namespace kusu_lib