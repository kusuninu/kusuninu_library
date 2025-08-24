#include <bits/stdc++.h>

//p must be a prime number
long long get_primitive_root(long long p) {
    if(p == 2) return 1;
    static thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<long long> dist(2, p - 1);
    std::vector<long long> soinsu;
    long long t = p - 1;
    for(long long i = 2; i * i <= t; i++) {
        if(t % i == 0) {
            soinsu.push_back(i);
            while(t % i == 0) t /= i;
        }
        if(t == 1) break;
    }
    if(t != 1) soinsu.push_back(t);
    while(true) {
        long long s = dist(rng);
        bool ok = true;
        for(auto a: soinsu) {
            long long power = (p - 1) / a;
            long long x = s;
            long long now = 1;
            while(power > 0) {
                if(power & 1) now *= x;
                now %= p;
                x *= x;
                x %= p;
                power >>= 1;
            }
            if(now == 1) {
                ok = false;
                break;
            }
        }
        if(ok) return s;
    }
    return -1;
}