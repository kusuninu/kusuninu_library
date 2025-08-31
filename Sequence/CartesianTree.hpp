#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<class T = long long, bool isMin = true> struct CartesianTree {
  public:
    CartesianTree() : N(0) {}
    CartesianTree(const std::vector<T>& V) : N(V.size()) {
        _left.resize(N, -1);
        _right.resize(N, -1);
        _parent.resize(N, -1);
        _range.resize(N);
        std::deque<int> st;
        for(int i = 0; i < N; i++) {
            int pre = -1;
            while(!st.empty() && (V[st.back()] < V[i] ^ isMin)) {
                pre = st.back();
                st.pop_back();
            }
            if(pre != -1) {
                _left[i] = pre;
                _parent[pre] = i;
            }
            if(!st.empty()) {
                _right[st.back()] = i;
                _parent[i] = st.back();
            }
            st.push_back(i);
        }
        root = st.front();
        dfs(root);
    }

    int get_root() { return root; }
    int left(int i) { return _left[i]; }
    int right(int i) { return _right[i]; }
    int parent(int i) { return _parent[i]; }
    std::pair<int, int> range(int i) { return _range[i]; }

  private:
    std::vector<int> _left, _right, _parent;
    std::vector<std::pair<int, int>> _range;
    int N;
    int root;

    void dfs(int i) {
        int l = i;
        int r = i;
        if(_left[i] != -1) {
            dfs(_left[i]);
            l = _range[_left[i]].first;
        }
        if(_right[i] != -1) {
            dfs(_right[i]);
            r = _range[_right[i]].second;
        }
        _range[i] = {l, r};
    }
};
}//namespace kusu_lib