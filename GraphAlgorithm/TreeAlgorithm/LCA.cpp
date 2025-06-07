#include <bits/stdc++.h>
template<std::integral index = int>
struct LCA {
    struct Doubling {
        std::vector<std::vector<int>> d;
        int n;//size
        long long M;//query <= M
        int h = 0;

        Doubling() : n(0), M(0) {}

        Doubling(const std::vector<int>& G, long long limit = (1LL << 61) - 1) : n(G.size()), M(limit) {
            for(int i = 0; i < n; i++) assert(0 <= G[i] && G[i] < n);
            while((1LL << h) <= M) h++;
            d.resize(h);
            d[0] = G;
            for(int i = 0; i < h - 1; i++) {
                d[i + 1].resize(n);
                for(int j = 0; j < n; j++) d[i + 1][j] = d[i][d[i][j]];
            }
        }

        int calc(int i, long long t) {
            assert(0 <= i && i < n);
            assert(0 <= t && t <= M);
            int pos = i;
            int j = 0;
            while(t > 0) {
                if(t & 1) pos = d[j][pos];
                t >>= 1;
                j++;
            }
            return pos;
        }
    };

  private:
    std::vector<int> parent;
    std::vector<int> depth;
    const int N;
    Doubling d;

  public:
    LCA() : N(0) {}

    LCA(const std::vector<std::vector<index>>& G, index root = 0) : N(G.size()) {
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
        v = d.calc(v, depth[v] - depth[u]);
        if(u == v) return u;
        for(int i = d.h - 1; i >= 0; i--) {
            if(d.d[i][u] != d.d[i][v]) {
                u = d.d[i][u];
                v = d.d[i][v];
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
        return d.calc(v, n);
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
    int ordered_path(int u,int v,int w){
        assert(0 <= u && u < N && 0 <= v && v < N && 0 <= w && w < N);
        int d1 = dist(u, v);
        int d2 = dist(v, w);
        int d3 = dist(w, u);
        if(d1 + d2 == d3) return d3;
        return -1;
    }
};