#include<bits/stdc++.h>
//T must have function named "xplus","xminus","yplus","yminus","get"
//"get" must return ans that is class U
// M is max query
template<class T,class U>
struct Mo_Algorithm{
    Mo_Algorithm(int N,const std::vector<std::pair<int,int>>& queries,T e){
        _Q = queries.size();
        _queries.resize(_Q);
        for(int i = 0;i < _Q;i++) _queries[i] = {queries[i],i};
        L = 0;
        while(L*L<_Q+1) L++;
        L = N / L + 1;
        std::sort(_queries.begin(),_queries.end(),[&](auto A,auto B){return comp(A.first,B.first);});
        ans.resize(_Q);
        int x = 0;
        int y = 0;
        T now = e;
        for(int i = 0;i < _Q;i++){
            int nx,ny,id;
            nx = _queries[i].first.first;
            ny = _queries[i].first.second;
            id = _queries[i].second;
            while(x<nx){
                now.xplus();
                x++;
            }
            while(nx<x){
                now.xminus();
                x--;
            }
            while(y<ny){
                now.yplus();
                y++;
            }
            while(ny<y){
                now.yminus();
                y--;
            }
            ans[id] = now.get();
        }
    }
    U operator[](int p){
        assert(0<=p&&p<_Q);
        return ans[p];
    }

    private:
    std::vector<std::pair<std::pair<int,int>,int>> _queries;
    int _Q;
    int L;//M/sqrt(_Q)
    std::vector<U> ans;
    bool comp(const std::pair<int,int>& a,const std::pair<int,int>& b){
        if(a.first/L<b.first/L) return true;
        if(a.first/L==b.first/L){
            if(a.first/L%2==0&&a.second<b.second) return true;
            else if(a.first/L%2==1&&b.second<a.second) return true;
        }
        return false;
    }
};