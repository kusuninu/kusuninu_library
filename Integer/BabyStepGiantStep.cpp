#include <bits/stdc++.h>
struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        // http://xorshift.di.unimi.it/splitmix64.c
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

//return min n s.t. f^n(x) = y & n <= M. if such n does not exist, return -1.
//this may return n s.t. f^n(x) = y & M < n.
//m should be sqrt(M)
template<class F>
long long BabyStepGiantStep(long long x, long long y, long long M,long long m,F F_INV,F F_M) {
    std::unordered_set<long long, custom_hash> hash;
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