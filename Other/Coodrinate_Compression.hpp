#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<typename T = long long> struct Coordinate_Compression {
    Coordinate_Compression() {}
    Coordinate_Compression(const std::vector<T>& V) { xs = V; }
    void add(T x) { xs.push_back(x); }
    void init() {
        std::sort(xs.begin(), xs.end());
        xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
        initialized = true;
    }
    //index of x
    int operator()(T x) {
        if(!initialized) init();
        return std::upper_bound(xs.begin(), xs.end(), x) - xs.begin() - 1;
    }
    //ith smallest number
    T operator[](int i) {
        if(!initialized) init();
        return xs[i];
    }
    int size() {
        if(!initialized) init();
        return xs.size();
    }

  private:
    bool initialized = false;
    std::vector<T> xs;
};
}//namespace kusu_lib