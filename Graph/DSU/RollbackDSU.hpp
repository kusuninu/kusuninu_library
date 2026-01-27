#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
//each function is O(log(N))
struct RollbackDSU {
  private:
    const int _N;
    //data[i] < 0 -> i is leader and size is -data[i]
    //data[i] >= 0 -> i's leader is data[i]
    std::vector<int> data;
    std::stack<std::pair<int, int>> history;
    int snap_time = 0;

  public:
    RollbackDSU() : _N(0) {};

    RollbackDSU(int N) : _N(N) {
        assert(_N >= 0);
        data.resize(_N, -1);
    }

    int merge(int x, int y) {
        assert(0 <= x && x < _N && 0 <= y && y < _N);
        x = leader(x);
        y = leader(y);
        if(data[x] > data[y]) std::swap(x, y);
        history.emplace(x, data[x]);
        history.emplace(y, data[y]);
        if(x == y) return y;
        data[x] += data[y];
        data[y] = x;
        return x;
    }

    bool same(int x, int y) {
        assert(0 <= x && x < _N && 0 <= y && y < _N);
        return leader(x) == leader(y);
    }

    int leader(int x) {
        assert(0 <= x && x < _N);
        if(data[x] < 0) return x;
        return leader(data[x]);
    }

    int size(int x) {
        assert(0 <= x && x < _N);
        return -data[leader(x)];
    }

    void undo() {
        assert(history.size() > 0);
        for(int i = 0; i < 2; ++i) {
            auto [x, d] = history.top();
            history.pop();
            data[x] = d;
        }
    }

    void snapshot() { snap_time = history.size() / 2; }

    //goal = -1 -> rollback to snapshot
    void rollback(int goal = -1) {
        if(goal == -1) goal = snap_time;
        goal *= 2;
        assert(goal <= int(history.size()));
        while(goal < int(history.size())) undo();
    }

    std::vector<std::vector<int>> groups() {
        std::vector<int> leader_buf(_N), group_size(_N);
        for(int i = 0; i < _N; i++) {
            leader_buf[i] = leader(i);
            group_size[leader_buf[i]]++;
        }
        std::vector<std::vector<int>> result(_N);
        for(int i = 0; i < _N; i++) result[i].reserve(group_size[i]);
        for(int i = 0; i < _N; i++) result[leader_buf[i]].push_back(i);
        result.erase(std::remove_if(result.begin(), result.end(),
                                    [&](const std::vector<int>& v) { return v.empty(); }),
                     result.end());
        return result;
    }
};
}//namespace kusu_lib