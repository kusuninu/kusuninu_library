#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
struct Combination {
  private:
    long long mod;
    long long n;
    std::vector<long long> _inv, _fact, _fact_inv;

  public:

    Combination() {}

    Combination(long long n, long long mod) : mod(mod), n(n) {
        _inv.resize(n + 1);
        _fact.resize(n + 1);
        _fact_inv.resize(n + 1);
        _inv[1] = 1;
        _fact[0] = 1;
        _fact_inv[0] = 1;
        for(long long i = 2; i <= n; i++) _inv[i] = mod - (mod / i) * _inv[mod % i] % mod;
        for(long long i = 1; i <= n; i++) _fact[i] = _fact[i - 1] * i % mod;
        for(long long i = 1; i <= n; i++) _fact_inv[i] = _fact_inv[i - 1] * _inv[i] % mod;
    }

    long long operator()(long long a, long long b) {
        if(a < 0) return 0;
        if(a < b) return 0;
        if(b < 0) return 0;
        assert(a <= n);
        return _fact[a] * _fact_inv[b] % mod * _fact_inv[a - b] % mod;
    }

    long long P(long long a, long long b) {
        assert(0 <= a && 0 <= b);
        if(a < b) return 0;
        assert(a <= n);
        return _fact[a] * _fact_inv[a - b] % mod;
    }

    long long fact(long long a) {
        assert(0 <= a && a <= n);
        return _fact[a];
    }

    long long inv(long long a) {
        assert(1 <= a && a <= n);
        return _inv[a];
    }

    long long fact_inv(long long a) {
        assert(0 <= a && a <= n);
        return _fact_inv[a];
    }
};
}//namespace kusu_lib