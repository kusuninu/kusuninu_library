#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {


    
//@return vector<array<{dist, from}>>
//O(N * K^2)
template<int K>
std::vector<std::array<std::pair<int, int>, K>> KthNearestBFS(const std::vector<std::vector<int>>& G,
                                                              int s = 0) {
    const long long INF = std::numeric_limits<int>::max() / 2;
    int N = G.size();
    std::queue<std::tuple<int, int, int>> q;//dist, pos, from
    q.emplace(0, s, s);
    std::vector<std::array<std::pair<int, int>, K>> dis(N);//dist, from
    std::vector<int> visited(N);
    dis[s][0] = {0, s};
    while(!q.empty()) {
        auto [d, p, f] = q.front();
        q.pop();
        for(int next: G[p]) {
            if(visited[next] == K) continue;
            bool br = false;
            for(int i = 0; i < visited[next]; i++) {
                if(dis[next][i].second == f) {
                    br = true;
                    break;
                }
            }
            if(br) continue;
            dis[next][visited[next]] = {d + 1, f};
            q.emplace(d + 1, next, f);
            visited[next]++;
        }
    }
    return dis;
}

//@return vector<array<{dist, from}>>
//O(N * K^2)
template<int K>
std::vector<std::array<std::pair<int, int>, K>> KthNearestBFS(const std::vector<std::vector<int>>& G,
                                                              const std::vector<int>& sources) {
    const long long INF = std::numeric_limits<int>::max() / 2;
    int N = G.size();
    std::queue<std::tuple<int, int, int>> q;//dist, pos, from
    std::vector<std::array<std::pair<int, int>, K>> dis(N);//dist, from
    std::vector<int> visited(N);
    for(int s: sources) {
        q.emplace(0, s, s);
        dis[s][0] = {0, s};
    }
    while(!q.empty()) {
        auto [d, p, f] = q.front();
        q.pop();
        for(int next: G[p]) {
            if(visited[next] == K) continue;
            bool br = false;
            for(int i = 0; i < visited[next]; i++) {
                if(dis[next][i].second == f) {
                    br = true;
                    break;
                }
            }
            if(br) continue;
            dis[next][visited[next]] = {d + 1, f};
            q.emplace(d + 1, next, f);
            visited[next]++;
        }
    }
    return dis;
}
}//namespace kusu_lib