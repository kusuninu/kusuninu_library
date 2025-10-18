#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
struct LowLink {
  private:
    const std::vector<std::vector<int>>& _G;
    int N;
    int* ord;
    int* low;
    int* articulation;
    bool* visited;
    int k = 0;
    int cc = 0;
    std::vector<std::pair<int, int>> bridges;
    std::vector<std::vector<int>> tecc;
    std::vector<int> tecc_idx;
    std::vector<std::vector<int>> tecc_tree;

    void dfs(int n, int p) {
        ord[n] = k;
        low[n] = k;
        k++;
        visited[n] = true;
        int c = 0;
        bool flag = false;
        for(int next: _G[n]) {
            if(next == p && !flag) {
                flag = true;
                continue;
            }
            if(!visited[next]) {
                dfs(next, n);
                c += ord[n] <= low[next];
                if(low[next] < low[n]) low[n] = low[next];
            }
            else {
                if(ord[next] < low[n]) low[n] = ord[next];
            }
        }
        if(p == -1) articulation[n] = c - 1;
        if(p != -1) articulation[n] = c;
    }

    void dfs2(int n) {
        visited[n] = true;
        tecc.back().push_back(n);
        tecc_idx[n] = (int)tecc.size() - 1;
        for(int next: _G[n]) {
            if(!visited[next] && !is_bridge(n, next)) dfs2(next);
        }
    }

    void bridge_initialize() {
        for(int i = 0; i < N; i++) {
            for(int j: _G[i]) {
                if(ord[j] <= ord[i]) continue;
                if(ord[i] < low[j]) bridges.emplace_back(i, j);
            }
        }
    }

    void tecc_initialize() {
        tecc_idx.resize(N);
        for(int i = 0; i < N; i++) visited[i] = false;
        for(int i = 0; i < N; i++) {
            if(!visited[i]) {
                tecc.push_back({});
                dfs2(i);
            }
        }
    }

    void tecc_tree_initialize() {
        if(bridges.empty()) bridge_initialize();
        if(tecc.empty()) tecc_initialize();
        tecc_tree.resize(tecc.size());
        for(auto [u, v]: bridges) {
            tecc_tree[tecc_idx[u]].push_back(tecc_idx[v]);
            tecc_tree[tecc_idx[v]].push_back(tecc_idx[u]);
        }
    }

  public:
    LowLink(const std::vector<std::vector<int>>& G) : _G(G) {
        N = G.size();
        ord = new int[N];
        low = new int[N];
        articulation = new int[N];
        visited = new bool[N];
        for(int i = 0; i < N; i++) visited[i] = false;
        for(int i = 0; i < N; i++) {
            if(!visited[i]) {
                dfs(i, -1);
                cc++;
            }
        }
    }

    //edge(u,v) must exist
    bool is_bridge(int u, int v) {
        assert(0 <= u && u < N && 0 <= v && v < N);
        if(ord[u] > ord[v]) std::swap(u, v);
        return ord[u] < low[v];
    }

    bool is_articulation_point(int p) {
        assert(0 <= p && p < N);
        return 0 < articulation[p];
    }

    int get_articulating_number(int p) {
        assert(0 <= p && p < N);
        return articulation[p];
    }

    std::vector<std::pair<int, int>> get_bridges() {
        if(bridges.empty()) bridge_initialize();
        return bridges;
    }

    std::vector<std::vector<int>> two_edge_connected_components() {
        if(tecc.empty()) tecc_initialize();
        return tecc;
    }

    std::vector<int> two_edge_connected_components_index() {
        if(tecc.empty()) tecc_initialize();
        return tecc_idx;
    }

    std::vector<std::vector<int>> two_edge_connected_components_tree() {
        if(tecc_tree.empty()) tecc_tree_initialize();
        return tecc_tree;
    }

    int count_connected_component() { return cc; }
};
}//namespace kusu_lib