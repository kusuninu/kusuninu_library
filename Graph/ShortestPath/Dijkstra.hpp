#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<class dist = long long>
std::vector<dist> Dijkstra(const std::vector<std::vector<std::pair<int, dist>>>& G, int s = 0) {
    using pp = std::pair<dist, int>;
    const auto INF = std::numeric_limits<dist>::max() / 2;
    int N = G.size();
    std::vector<dist> dis(N, INF);
    dis[s] = 0;
    std::priority_queue<pp, std::vector<pp>, std::greater<pp>> pq;
    pq.push({0, s});
    while(!pq.empty()) {
        auto p = pq.top();
        pq.pop();
        dist d = p.first;
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

template<class dist = long long>
std::vector<dist> Dijkstra(const std::vector<std::vector<std::pair<int, dist>>>& G,
                           const std::vector<int>& sources) {
    using pp = std::pair<dist, int>;
    const auto INF = std::numeric_limits<dist>::max() / 2;
    int N = G.size();
    std::vector<dist> dis(N, INF);
    std::vector<pp> make_pq(sources.size());
    for(int i = 0; int s: sources) {
        dis[s] = 0;
        make_pq[i] = {0, s};
        i++;
    }
    std::priority_queue<pp, std::vector<pp>, std::greater<pp>> pq(make_pq);
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