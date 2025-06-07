#include <bits/stdc++.h>
template<std::integral index = int>
std::vector<int> BFS01(const std::vector<std::vector<std::pair<index, index>>>& G, index s) {
    const auto INF = numeric_limits<int>::max();
    int N = G.size();
    std::deque<int> d;
    d.push_back(s);
    std::vector<int> dis(N, INF);
    dis[s] = 0;
    while(!d.empty()) {
        int p = d.back();
        d.pop_back();
        for(auto [to, c]: G[p]) {
            if(dis[to] != INF) continue;
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