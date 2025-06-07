#include <bits/stdc++.h>
template<std::integral index = int>
std::vector<int> BFS(const std::vector<std::vector<index>>& G, index s) {
    const long long INF = std::numeric_limits<int>::max();
    int N = G.size();
    std::queue<int> q;
    q.push(s);
    std::vector<int> dis(N, INF);
    dis[s] = 0;
    while(!q.empty()){
        int p = q.front();
        q.pop();
        for(int next : G[p]){
            if(dis[next]!=INF) continue;
            dis[next] = dis[p] + 1;
            q.push(next);
        }
    }
    return dis;
}