#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<class T = long long> std::vector<T> ContinuedFractionExpansion(T a, T b) {
    assert(0 <= a && 0 < b);
    std::vector<T> res;
    while(b != 0) {
        res.push_back(a / b);
        a %= b;
        swap(a, b);
    }
    return res;
}
}//namespace kusu_lib