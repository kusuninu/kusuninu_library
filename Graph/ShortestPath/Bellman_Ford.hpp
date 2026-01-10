#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
//negative loop -> true
template<class dist = long long>
bool Bellman_Ford(const std::vector<std::vector<std::pair<int, dist>>>& G,
                  std::vector<dist>& dis) {
    assert(G.size() == dis.size());
    const auto INF = std::numeric_limits<dist>::max() / 2;
    int N = G.size();
    for(int i = 0; i < N; i++) {
        bool finished = true;
        for(int j = 0; j < N; j++) {
            for(auto [t, c]: G[j]) {
                if(dis[j] < INF && dis[j] + c < dis[t]) {
                    dis[t] = dis[j] + c;
                    finished = false;
                }
            }
        }
        if(i == N - 1 && !finished) return true;
        if(finished) break;
    }
    return false;
}
}//namespace kusu_lib