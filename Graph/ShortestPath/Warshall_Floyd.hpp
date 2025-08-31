#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<std::integral dist = long long> void Warshall_Floyd(std::vector<std::vector<dist>>& dis) {
    int V = dis.size();
    for(int k = 0; k < V; k++) {
        for(int i = 0; i < V; i++) {
            for(int j = 0; j < V; j++) { dis[i][j] = std::min(dis[i][j], dis[i][k] + dis[k][j]); }
        }
    }
}
}//namespace kusu_lib