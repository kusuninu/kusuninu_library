#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
std::vector<int> BFS01(const std::vector<std::vector<std::pair<int, int>>>& G, int s = 0) {
    const auto INF = std::numeric_limits<int>::max() / 2;
    int N = G.size();
    std::deque<int> d;
    d.push_back(s);
    std::vector<int> dis(N, INF);
    dis[s] = 0;
    while(!d.empty()) {
        int p = d.back();
        d.pop_back();
        for(auto [to, c]: G[p]) {
            if(dis[to] <= dis[p] + c) continue;
            if(c == 0) {
                dis[to] = dis[p];
                d.push_back(to);
            } else if(c == 1) {
                dis[to] = dis[p] + 1;
                d.push_front(to);
            } else
                assert(true);
        }
    }
    return dis;
}

std::vector<int> BFS01(const std::vector<std::vector<std::pair<int, int>>>& G,
                                  const std::vector<int>& sources) {
    const auto INF = std::numeric_limits<int>::max() / 2;
    int N = G.size();
    std::deque<int> d;
    std::vector<int> dis(N, INF);
    for(int s: sources) {
        d.push_back(s);
        dis[s] = 0;
    }
    while(!d.empty()) {
        int p = d.back();
        d.pop_back();
        for(auto [to, c]: G[p]) {
            if(dis[to] <= dis[p] + c) continue;
            if(c == 0) {
                dis[to] = dis[p];
                d.push_back(to);
            } else if(c == 1) {
                dis[to] = dis[p] + 1;
                d.push_front(to);
            } else
                assert(true);
        }
    }
    return dis;
}
}//namespace kusu_lib