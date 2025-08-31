#include<bits\stdc++.h>
#include<atcoder/all>
template <class S, S (*op)(S, S), S (*e)()>
struct EulerTour_LCA{
    const int size;
    std::vector<S> v;
    static std::pair<int,int> op1(std::pair<int,int> a,std::pair<int,int> b){return std::min(a,b);}
    static std::pair<int,int> e1(){return {1<<29,0};}
    atcoder::segtree<std::pair<int,int>,op1,e1> LCA_seg;
    atcoder::segtree<S,op,e> subtree_seg;
    EulerTour_LCA(std::vector<std::vector<int>> t,std::vector<S> _v) : tree(t),size(t.size()){
        in.resize(size,-1);
        out.resize(size,-1);
        depth.resize(size,-1);
        in_depth.resize(size*2,1<<29);
        v = _v;
        dfs(0,-1);
        for(int i = 0;i < size;i++) in_depth[in[i]] = depth[i];
        std::vector<std::pair<int,int>> v(size*2);
        for(int i = 0;i < size;i++){
            v[in[i]] = {depth[i],i};
        }
        LCA_seg = atcoder::segtree<std::pair<int,int>,op1,e1>(v);
    }
    std::pair<int,int> LCA(int x,int y){
        auto p = LCA_seg.prod(std::min(in[x],in[y]),std::max(in[x],in[y])+1);
        return {p.second,p.first};
    }

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