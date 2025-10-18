#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
struct Combination {
  private:
    long long mod;
    long long n;
    std::vector<long long> _inv, _fact, _fact_inv;

  public:
    Combination(long long n = 1, long long mod = 2) : mod(mod), n(n) {
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

template<std::size_t N, long long MOD> struct CombinationStatic {
  private:
    std::array<long long, N + 1> _inv{};
    std::array<long long, N + 1> _fact{};
    std::array<long long, N + 1> _fact_inv{};

  public:
    constexpr CombinationStatic() {
        if constexpr(N >= 1) _inv[1] = 1;
        _fact[0] = 1;
        _fact_inv[0] = 1;
        for(std::size_t i = 2; i <= N; i++) _inv[i] = MOD - (MOD / i) * _inv[MOD % i] % MOD;
        for(std::size_t i = 1; i <= N; i++) _fact[i] = _fact[i - 1] * i % MOD;
        for(std::size_t i = 1; i <= N; i++) _fact_inv[i] = _fact_inv[i - 1] * _inv[i] % MOD;
    }

    constexpr long long operator()(long long a, long long b) const {
        if(a < 0 || b < 0 || a < b) return 0;
        assert(a <= (long long)N);
        return _fact[a] * _fact_inv[b] % MOD * _fact_inv[a - b] % MOD;
    }

    constexpr long long P(long long a, long long b) const {
        if(a < 0 || b < 0 || a < b) return 0;
        assert(a <= (long long)N);
        return _fact[a] * _fact_inv[a - b] % MOD;
    }

    constexpr long long fact(long long a) const {
        assert(0 <= a && a <= (long long)N);
        return _fact[a];
    }

    constexpr long long inv(long long a) const {
        assert(1 <= a && a <= (long long)N);
        return _inv[a];
    }

    constexpr long long fact_inv(long long a) const {
        assert(0 <= a && a <= (long long)N);
        return _fact_inv[a];
    }
};
}//namespace kusu_lib