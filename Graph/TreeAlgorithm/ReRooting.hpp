#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<class S, S (*op)(S, S), S (*e)(), S (*add_root)(S, int)>
struct ReRooting {
  private:
    std::vector<std::vector<int>> G;
    int N;
    std::vector<std::vector<S>> dp;
    std::vector<S> ans;

    S dfs1(int n, int p) {
        S sum = e();
        for(int idx = -1; int next: G[n]) {
            idx++;
            if(next == p) continue;
            dp[n][idx] = dfs1(next, n);
            sum = op(sum, dp[n][idx]);
        }
        sum = add_root(sum, n);
        return sum;
    }

    void dfs2(int n, int p, S x) {
        for(int idx = 0; int next: G[n]) {
            if(next == p) {
                dp[n][idx] = x;
                break;
            }
            idx++;
        }
        int M = G[n].size();
        std::vector<S> sum_l(M + 1, e()), sum_r(M + 1, e());
        for(int i = 0; i < M; i++) {
            sum_l[i + 1] = op(sum_l[i], dp[n][i]);
            sum_r[M - i - 1] = op(sum_r[M - i], dp[n][M - i - 1]);
        }
        ans[n] = add_root(sum_l[M], n);
        for(int i = 0; i < M; i++) {
            if(G[n][i] == p) continue;
            dfs2(G[n][i], n, add_root(op(sum_l[i], sum_r[i + 1]), n));
        }
    }

  public:
    template<std::integral index>
    ReRooting(const std::vector<std::vector<index>>& G) : N(G.size()) {
        this->G.resize(N);
        for(int i = 0; i < N; i++) {
            this->G[i].resize(G[i].size());
            for(int j = 0; j < (int)G[i].size(); j++) this->G[i][j] = G[i][j];
        }
        dp.resize(N);
        for(int i = 0; i < N; i++) dp[i].resize(G[i].size(), e());
        ans.resize(N, e());
        dfs1(0, -1);
        dfs2(0, -1, e());
    }

    std::vector<S> get_ans() { return ans; }

    int size() { return N; }
};
}//namespace kusu_lib