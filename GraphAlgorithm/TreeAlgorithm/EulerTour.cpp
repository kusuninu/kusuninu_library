#include<bits\stdc++.h>
#include<atcoder/all>
struct EulerTour_LCA{
    const int size;
    
    private:
    std::vector<std::vector<int>> tree;
    std::vector<int> in,out,depth,in_depth;
    
    int now = 0;
    int d = 0;
    void dfs(int n,int p){
        in[n] = now;
        depth[n] = d;
        for(int next : tree[n]){
            if(next == p) continue;
            now++;
            d++;
            dfs(next,n);
        }
        out[n] = now;
        now++;
        d--;
    }
};