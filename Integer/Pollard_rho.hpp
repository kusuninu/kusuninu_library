#pragma once
#include "../String/RunLengthEncoding.hpp"
#include "MillerRabin.hpp"
#include "Montgomery.hpp"
#include <bits/stdc++.h>

namespace kusu_lib {
template<class T> T Pollard_rho(T N) {
    static_assert(std::is_unsigned<T>::value);
    assert(2 <= N);
    if(N % 2 == 0) return 2;
    if(is_prime(N)) return N;
    static thread_local std::mt19937 rng(std::random_device{}());
    using mint = Montgomery<T>::MontgomeryInt;
    Montgomery<T> mt(N);
    std::uniform_int_distribution<T> dist(1, N - 1);
    mint c(&mt);
    mint one(1, &mt);
    auto f = [&](mint x) { return x * x + c; };
    int d = sizeof(T) * 8 - std::countl_zero(N);
    int m = 1 << (d / 8);
    while(true) {
        c = dist(rng);
        mint x(&mt), y(&mt), ys(&mt);
        mint q = one;
        y = dist(rng);
        T g = 1;
        for(int r = 1; g == 1; r <<= 1) {
            x = y;
            for(int i = 0; i < r; i++) y = f(y);
            for(int k = 0; k < r; k += m) {
                ys = y;
                for(int i = 0; i < std::min(m, r - k); i++) {
                    y = f(y);
                    q *= x - y;
                }
                g = std::gcd(q.val(), N);
                if(g != 1) break;
            }
        }
        if(g == N) {
            g = 1;
            while(g == 1) {
                ys = f(ys);
                g = std::gcd((x - ys).val(), N);
            }
        }
        if(g != N) return g;
    }
    return -1;
}

std::vector<std::pair<long long, int>> Pollard_rho_factorize(long long N) {
    assert(1 <= N && N < (1LL << 62));
    if(N == 1) return {};
    std::stack<long long> st;
    st.push(N);
    std::vector<long long> res;
    while(!st.empty()) {
        long long a = st.top();
        st.pop();
        if(is_prime(a)) {
            res.push_back(a);
            continue;
        }
        long long p;
        if(a < (1LL << 30)) { p = Pollard_rho<unsigned int>(a); }
        else { p = Pollard_rho<unsigned long long>(a); }
        st.push(p);
        st.push(a / p);
    }
    std::sort(res.begin(), res.end());
    return RunLengthEncoding(res);
}

}//namespace kusu_lib