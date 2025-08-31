#pragma once
#include <bits/stdc++.h>
#include "../Doubling.hpp"
namespace kusu_lib {
struct LCA {
  private:
    std::vector<int> parent;
    std::vector<int> depth;
    const int N;
    Doubling d;

  public:
    LCA() : N(0) {}

    LCA(const std::vector<std::vector<int>>& G, int root = 0) : N(G.size()) {
        assert(0 <= root && root < N);
        parent.resize(N + 1);
        parent[root] = N;
        parent[N] = N;
        depth.resize(N);
        std::function<void(int)> dfs = [&](int pos) {
            for(int next: G[pos]) {
                if(next == parent[pos]) continue;
                parent[next] = pos;
                depth[next] = depth[pos] + 1;
                dfs(next);
            }
        };
        dfs(root);
        d = Doubling(parent, N);
    }

    int get_LCA(int u, int v) {
        assert(0 <= u && u < N && 0 <= v && v < N);
        if(depth[u] > depth[v]) std::swap(u, v);
        v = d.move(v, depth[v] - depth[u]);
        if(u == v) return u;
        for(int i = d.height() - 1; i >= 0; i--) {
            if(d.move_2p(i,u) != d.move_2p(i,v)) {
                u = d.move_2p(i,u);
                v = d.move_2p(i,v);
            }
        }
        return parent[u];
    }

    int dist(int u, int v) {
        assert(0 <= u && u < N && 0 <= v && v < N);
        int t = get_LCA(u, v);
        return depth[u] + depth[v] - 2 * depth[t];
    }

    //if the ancestor does not exist, return -1
    int get_ancestor(int v, int n) {
        assert(0 <= n);
        assert(0 <= v && v < N);
        if(depth[v] < n) return -1;
        return d.move(v, n);
    }

    int get_depth(int v) {
        assert(0 <= v && v < N);
        return depth[v];
    }

    //if straight path does not exist, return -1
    int straight_path(int u, int v, int w) {
        assert(0 <= u && u < N && 0 <= v && v < N && 0 <= w && w < N);
        int d1 = dist(u, v);
        int d2 = dist(v, w);
        int d3 = dist(w, u);
        if(d1 + d2 == d3) return d3;
        if(d2 + d3 == d1) return d1;
        if(d3 + d1 == d2) return d2;
        return -1;
    }

    //path u => v => w
    int ordered_path(int u, int v, int w) {
        assert(0 <= u && u < N && 0 <= v && v < N && 0 <= w && w < N);
        int d1 = dist(u, v);
        int d2 = dist(v, w);
        int d3 = dist(w, u);
        if(d1 + d2 == d3) return d3;
        return -1;
    }
};
}//namespace kusu_lib