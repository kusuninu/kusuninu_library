#pragma once
#include "BabyStepGiantStep.hpp"
#include "MillerRabin.hpp"
#include "Montgomery.hpp"
#include "Pollard_rho.hpp"
#include <bits/stdc++.h>
namespace kusu_lib {
constexpr long long safe_mod(long long x, long long m) {
    x %= m;
    if(x < 0) x += m;
    return x;
}
//from atcoder_library
// @param b `1 <= b`
// @return pair(g, x) s.t. g = gcd(a, b), xa = g (mod b), 0 <= x < b/g
constexpr std::pair<long long, long long> inv_gcd(long long a, long long b) {
    a = safe_mod(a, b);
    if(a == 0) return {b, 0};

    // Contracts:
    // [1] s - m0 * a = 0 (mod b)
    // [2] t - m1 * a = 0 (mod b)
    // [3] s * |m1| + t * |m0| <= b
    long long s = b, t = a;
    long long m0 = 0, m1 = 1;

    while(t) {
        long long u = s / t;
        s -= t * u;
        m0 -= m1 * u;// |m1 * u| <= |m1| * s <= b

        // [3]:
        // (s - t * u) * |m1| + t * |m0 - m1 * u|
        // <= s * |m1| - t * u * |m1| + t * (|m0| + |m1| * u)
        // = s * |m1| + t * |m0| <= b

        auto tmp = s;
        s = t;
        t = tmp;
        tmp = m0;
        m0 = m1;
        m1 = tmp;
    }
    // by [3]: |m0| <= b/g
    // by g != b: |m0| < b/g
    if(m0 < 0) m0 += b / s;
    return {s, m0};
}

//from atcoder_library
long long mod_inv(long long x, long long m) {
    assert(1 <= m);
    auto z = inv_gcd(x, m);
    assert(z.first == 1);
    return z.second;
}

//O(sqrt(N))
std::vector<std::pair<long long, int>> trial_division(long long N) {
    std::vector<std::pair<long long, int>> res;
    for(long long i = 2; i * i <= N; i++) {
        if(N % i == 0) {
            int p = 0;
            while(N % i == 0) {
                N /= i;
                p++;
            }
            res.emplace_back(i, p);
        }
    }
    if(N != 1) res.emplace_back(N, 1);
    return res;
}

//O(N^0.25)
std::vector<std::pair<long long, int>> prime_factorize(long long N) {
    assert(1 <= N);
    if(N <= 5000000) return trial_division(N);
    return Pollard_rho_factorize(N);
}

std::vector<long long> get_divisors(long long N) {
    assert(1 <= N);
    auto factors = prime_factorize(N);
    std::vector<long long> res = {1};
    std::stack<long long> st;
    for(auto pf: factors) {
        for(int a: res) {
            for(int i = 0; i < pf.second; i++) {
                a *= pf.first;
                st.push(a);
            }
        }
        while(!st.empty()) {
            res.push_back(st.top());
            st.pop();
        }
    }
    return res;
}

//O(sqrt(N))
std::vector<long long> get_prime_factors(long long N) {
    std::vector<long long> res;
    if(N <= 5000000) {
        long long t = N;
        for(long long i = 2; i * i <= t; i++) {
            if(t % i == 0) {
                res.push_back(i);
                while(t % i == 0) t /= i;
            }
            if(t == 1) break;
        }
        if(t != 1) res.push_back(t);
    }
    else {
        auto V = prime_factorize(N);
        for(auto [p, q]: V) res.push_back(p);
    }
    return res;
}

// @return gcd(a,b)
// a*x + b*y = 1
long long extGCD(long long a, long long b, long long& x, long long& y) {
    if(b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long d = extGCD(b, a % b, y, x);
    y -= a / b * x;
    return d;
}

//from atcoder_library
// (rem, mod)
//@return {ans, lcm}
std::pair<long long, long long> ChineseRemTheorem(const std::vector<long long>& r,
                                                  const std::vector<long long>& m) {
    assert(r.size() == m.size());
    int n = int(r.size());
    // Contracts: 0 <= r0 < m0
    long long r0 = 0, m0 = 1;
    for(int i = 0; i < n; i++) {
        assert(1 <= m[i]);
        long long r1 = safe_mod(r[i], m[i]), m1 = m[i];
        if(m0 < m1) {
            std::swap(r0, r1);
            std::swap(m0, m1);
        }
        if(m0 % m1 == 0) {
            if(r0 % m1 != r1) return {0, 0};
            continue;
        }
        // assume: m0 > m1, lcm(m0, m1) >= 2 * max(m0, m1)

        // (r0, m0), (r1, m1) -> (r2, m2 = lcm(m0, m1));
        // r2 % m0 = r0
        // r2 % m1 = r1
        // -> (r0 + x*m0) % m1 = r1
        // -> x*u0*g % (u1*g) = (r1 - r0) (u0*g = m0, u1*g = m1)
        // -> x = (r1 - r0) / g * inv(u0) (mod u1)

        // im = inv(u0) (mod u1) (0 <= im < u1)
        long long g, im;
        std::tie(g, im) = inv_gcd(m0, m1);

        long long u1 = (m1 / g);
        // |r1 - r0| < (m0 + m1) <= lcm(m0, m1)
        if((r1 - r0) % g) return {0, 0};

        // u1 * u1 <= m1 * m1 / g / g <= m0 * m1 / g = lcm(m0, m1)
        long long x = (r1 - r0) / g % u1 * im % u1;

        // |r0| + |m0 * x|
        // < m0 + m0 * (u1 - 1)
        // = m0 + m0 * m1 / g - m0
        // = lcm(m0, m1)
        r0 += x * m0;
        m0 *= u1;// -> lcm(m0, m1)
        if(r0 < 0) r0 += m0;
    }
    return {r0, m0};
}
//m * m < 2^63
template<class T = long long> T mod_pow(T a, T n, T m) {
    T res = 1;
    while(n > 0) {
        if(n & 1) res = res * a % m;
        a = a * a % m;
        n >>= 1;
    }
    return res;
}

long long montgomery_pow(long long a, long long n, long long m) {
    assert(3 <= m && m % 2 == 1);
    if(m < (1LL << 30)) {
        Montgomery<unsigned int> mt(m);
        Montgomery<unsigned int>::MontgomeryInt x(a, &mt);
        return x.pow(n).val();
    }
    Montgomery<unsigned long long> mt(m);
    Montgomery<unsigned long long>::MontgomeryInt x(a, &mt);
    return x.pow(n).val();
}

template<class T = long long> T integer_pow(T a, T n) {
    T res = 1;
    while(n > 0) {
        if(n & 1) res = res * a;
        a = a * a;
        n >>= 1;
    }
    return res;
}

//p must be a prime number
long long get_primitive_root(long long p) {
    if(p == 2) return 1;
    static thread_local std::mt19937 rng(std::random_device{}());
    Montgomery<unsigned long long> mt(p);
    std::uniform_int_distribution<long long> dist(2, p - 1);
    std::vector<long long> soinsu = get_prime_factors(p - 1);
    while(true) {
        Montgomery<unsigned long long>::MontgomeryInt s(dist(rng), &mt);
        bool ok = true;
        for(auto a: soinsu) {
            long long power = (p - 1) / a;
            if(s.pow(power).val() == 1) {
                ok = false;
                break;
            }
        }
        if(ok) return s.val();
    }
    return -1;
}

//return -1 if n s.t. a^n=x does not exist
long long DiscreteLog(long long a, long long x, int p) {
    a = safe_mod(a, p);
    x = safe_mod(x, p);
    if(p == 1) return 0;
    if(x == 1) return 0;
    int d = 32 - std::countr_zero((unsigned)p);
    long long apow = 1;
    for(int i = 0; i < d; i++) {
        if(apow == x) return i;
        apow = apow * a % p;
    }
    int g = std::gcd(apow, p);
    if(x % g != 0) return -1;
    p /= g;
    x = x * mod_inv(apow, p) % p;
    a %= p;
    if(p == 1) return 0;
    if(x == 1) return 0;
    long long m = sqrtl(p);
    while(m * m <= p) m++;
    while(p < m * m) m--;
    long long inv = mod_inv(a, p);
    long long am = mod_pow<long long>(a, m, p);
    std::function<long long(long long)> F_INV = [&](long long b) { return b * inv % p; };
    std::function<long long(long long)> F_M = [&](long long b) { return b * am % p; };
    long long res = BabyStepGiantStep(a, x, p - 1, m, F_INV, F_M);
    if(res == -1) return -1;
    return res + 1 + d;
}

}//namespace kusu_lib