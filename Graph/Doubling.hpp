#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
struct Doubling {
  public:
    Doubling() : n(0), M(0) {}

    Doubling(const std::vector<int>& G,
             unsigned long long limit = std::numeric_limits<unsigned long long>::max())
        : n(G.size()), M(limit) {
        for(int i = 0; i < n; i++) assert(0 <= G[i] && G[i] < n);
        h = 64 - std::__countl_zero(M);
        d.resize(h);
        d[0] = G;
        for(int i = 0; i < h - 1; i++) {
            d[i + 1].resize(n);
            for(int j = 0; j < n; j++) d[i + 1][j] = d[i][d[i][j]];
        }
    }

    int move(int i, unsigned long long t) {
        assert(0 <= i && i < n);
        assert(t <= M);
        int pos = i;
        int j = 0;
        while(t > 0) {
            if(t & 1) pos = d[j][pos];
            t >>= 1;
            j++;
        }
        return pos;
    }

    int move_2p(int i, int p) {
        assert(0 <= i && i < n);
        assert(0 <= p && p < h);
        return d[p][i];
    }

    int height() { return h; }

  private:
    std::vector<std::vector<int>> d;
    int n;//size
    unsigned long long M;//query <= M
    int h = 0;
};
}//namespace kusu_lib