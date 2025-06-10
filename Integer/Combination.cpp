#include <bits/stdc++.h>
struct Combination {
  private:
    const long long mod;
    const long long n;
    std::vector<long long> inv, _fact, fact_inv;

  public:
    Combination(long long n, long long mod) : mod(mod), n(n) {
        inv.resize(n + 1);
        _fact.resize(n + 1);
        fact_inv.resize(n + 1);
        inv[1] = 1;
        _fact[0] = 1;
        fact_inv[0] = 1;
        for(long long i = 2; i <= n; i++) inv[i] = mod - (mod / i) * inv[mod % i] % mod;
        for(long long i = 1; i <= n; i++) _fact[i] = _fact[i - 1] * i % mod;
        for(long long i = 1; i <= n; i++) fact_inv[i] = fact_inv[i - 1] * inv[i] % mod;
    }
    long long operator()(long long a, long long b) {
        if(a < 0) return 0;
        if(a < b) return 0;
        if(b < 0) return 0;
        assert(a <= n);
        return _fact[a] * fact_inv[b] % mod * fact_inv[a - b] % mod;
    }
    long long P(long long a, long long b) {
        assert(0 <= a && 0 <= b);
        if(a < b) return 0;
        assert(a <= n);
        return _fact[a] * fact_inv[a - b] % mod;
    }
    long long fact(long long a) {
        assert(0 <= a && a <= n);
        return _fact[a];
    }
};