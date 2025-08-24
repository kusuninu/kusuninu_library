#include <bits/stdc++.h>
struct TreeHash {
  public:
    TreeHash(const std::vector<std::vector<int>>& G, int root = 0) : G(G) {
        static thread_local std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<unsigned long long> dist(1, (1ULL << 61) - 2);
        while(depth2rand.size() < G.size()) depth2rand.push_back(dist(rng));
        hash.resize(G.size());
        dfs(root, -1);
    }

    unsigned long long get(int i){return hash[i];}

  private:
    using ull = unsigned long long;
    const std::vector<std::vector<int>>& G;
    std::vector<ull> hash;
    static std::vector<unsigned long long> depth2rand;

    int dfs(int n, int p) {
        ull res = 1;
        int d = 0;
        for(auto next: G[n]) {
            if(next == p) continue;
            d = std::max(d, dfs(next, n) + 1);
            res = Mul(res, hash[next]);
        }
        res += depth2rand[d];
        if(res > MOD) res -= MOD;
        hash[n] = res;
        return d;
    }

    static const ull MASK30 = (1ULL << 30) - 1;
    static const ull MASK31 = (1ULL << 31) - 1;
    static const ull MOD = (1ULL << 61) - 1;
    static const ull POSITIVIZER = MOD * 4;
    static const ull MASK61 = MOD;
    static ull base;
    static std::vector<ull> powmemo;
    // a*b mod 2^61-1
    static ull Mul(ull a, ull b) {
        ull au = a >> 31;
        ull ad = a & MASK31;
        ull bu = b >> 31;
        ull bd = b & MASK31;
        ull mid = ad * bu + au * bd;
        ull midu = mid >> 30;
        ull midd = mid & MASK30;
        return CalcMod(au * bu * 2 + midu + (midd << 31) + ad * bd);
    }

    // mod 2^61-1
    static ull CalcMod(ull x) {
        ull xu = x >> 61;
        ull xd = x & MASK61;
        ull res = xu + xd;
        if(res >= MOD) res -= MOD;
        return res;
    }
};
std::vector<unsigned long long> TreeHash::depth2rand;