#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
std::vector<int> BFS(const std::vector<std::vector<int>>& G, int s = 0) {
    const int INF = std::numeric_limits<int>::max() / 2;
    int N = G.size();
    std::queue<int> q;
    q.push(s);
    std::vector<int> dis(N, INF);
    dis[s] = 0;
    while(!q.empty()) {
        int p = q.front();
        q.pop();
        for(int next: G[p]) {
            if(dis[next] != INF) continue;
            dis[next] = dis[p] + 1;
            q.push(next);
        }
    }
    return dis;
}

std::vector<int> BFS(const std::vector<std::vector<int>>& G,
                                const std::vector<int>& sources) {
    const int INF = std::numeric_limits<int>::max() / 2;
    int N = G.size();
    std::queue<int> q;
    std::vector<int> dis(N, INF);
    for(int s: sources) {
        q.push(s);
        dis[s] = 0;
    }
    while(!q.empty()) {
        int p = q.front();
        q.pop();
        for(int next: G[p]) {
            if(dis[next] != INF) continue;
            dis[next] = dis[p] + 1;
            q.push(next);
        }
    }
    return dis;
}
}//namespace kusu_lib