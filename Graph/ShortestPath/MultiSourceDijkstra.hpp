#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<class dist = long long>
std::vector<dist> Dijkstra(const std::vector<std::vector<std::pair<int, dist>>>& G,
                           const std::vector<int>& sources) {
    using pp = std::pair<dist, int>;
    const auto INF = std::numeric_limits<dist>::max();
    int N = G.size();
    std::vector<dist> dis(N, INF);
    std::priority_queue<pp, std::vector<pp>, std::greater<pp>> pq;
    for(int s: sources) {
        dis[s] = 0;
        pq.push({0, s});
    }
    while(!pq.empty()) {
        auto p = pq.top();
        pq.pop();
        long long d = p.first;
        int n = p.second;
        if(dis[n] < d) continue;
        for(auto& e: G[n]) {
            if(d + e.second < dis[e.first]) {
                dis[e.first] = d + e.second;
                pq.emplace(d + e.second, e.first);
            }
        }
    }
    return dis;
}
}//namespace kusu_lib