#include <bits/stdc++.h>
//each function is O(log(N))
struct PartiallyPersistentDSU {
  private:
    const int _N;
    int* par;
    int* updated;
    int* _size;
    std::vector<std::vector<std::pair<int, int>>> size_changed;
    const int inf = 1 << 30;
    int now = 0;
    int* leader_memo;
    int* size_memo;

    int find_par(int t, int x) {
        if(t < updated[x]) return x;
        return find_par(t, par[x]);
    }

  public:
    PartiallyPersistentDSU() : _N(0) {};

    PartiallyPersistentDSU(int N) : _N(N) {
        assert(_N >= 0);
        par = new int[_N];
        updated = new int[_N];
        _size = new int[_N];
        size_changed.resize(_N);
        leader_memo = new int[_N];
        size_memo = new int[_N];
        for(int i = 0; i < _N; i++) {
            par[i] = -1;
            updated[i] = inf;
            _size[i] = 1;
            size_changed[i].emplace_back(0, 1);
        }
    }

    int merge(int x, int y) {
        assert(0 <= x && x < _N && 0 <= y && y < _N);
        now++;
        x = find_par(now, x);
        y = find_par(now, y);
        if(x == y) return y;
        if(_size[x] > _size[y]) std::swap(x, y);
        par[x] = y;
        _size[y] += _size[x];
        updated[x] = now;
        size_changed[y].emplace_back(now, _size[y]);
        return y;
    }

    bool same(int t, int x, int y) {
        assert(0 <= t && t <= now && 0 <= x && x < _N && 0 <= y && y < _N);
        return find_par(t, x) == find_par(t, y);
    }

    int leader(int t, int x) {
        assert(0 <= t && t <= now && 0 <= x && x < _N);
        return find_par(t, x);
    }

    int size(int t, int x) {
        assert(0 <= t && t <= now && 0 <= x && x < _N);
        return std::lower_bound(size_changed[x].begin(), size_changed[x].end(), std::make_pair(t, inf))
            ->second;
    }

    int time() { return now; }

    std::vector<std::vector<int>> groups(int t) {
        assert(0 <= t && t <= now);
        for(int i = 0; i < _N; i++) size_memo[i] = 0;
        for(int i = 0; i < _N; i++) {
            leader_memo[i] = find_par(t, i);
            size_memo[leader_memo[i]]++;
        }
        std::vector<std::vector<int>> res(_N);
        for(int i = 0; i < _N; i++) res[i].reserve(size_memo[i]);
        for(int i = 0; i < _N; i++) res[i].push_back(leader_memo[i]);
        res.erase(std::remove_if(res.begin(), res.end(), [](const auto& v) { return v.empty(); }), res.end());
        return res;
    }
};