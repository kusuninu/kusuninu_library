#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
std::vector<int> MultiSourceBFS(const std::vector<std::vector<int>>& G,
                                const std::vector<int>& sources) {
    const long long INF = std::numeric_limits<int>::max();
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