#include <bits/stdc++.h>

template<std::integral index = int>
std::vector<int> MultiSourceBFS01(const std::vector<std::vector<std::pair<index, index>>>& G,
                                  const std::vector<index>& sources) {
    const auto INF = numeric_limits<int>::max();
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