#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {

template<class T, bool isStrict = true>
requires requires(T a, T b) { a < b; }
std::vector<int> LongestIncreasingSubsequence(const std::vector<T>& V) {
    std::vector<T> tail_value;
    std::vector<int> tail_index;
    std::vector<int> prev(V.size(), -1);
    for(int i = 0; i < int(V.size()); i++) {
        typename std::vector<T>::iterator it;
        if constexpr(isStrict) { it = std::lower_bound(tail_value.begin(), tail_value.end(), V[i]); }
        else { it = std::upper_bound(tail_value.begin(), tail_value.end(), V[i]); }
        int pos = it - tail_value.begin();
        if(it == tail_value.end()) {
            tail_value.push_back(V[i]);
            tail_index.push_back(i);
        }
        else {
            *it = V[i];
            tail_index[pos] = i;
        }
        if(pos != 0) prev[i] = tail_index[pos - 1];
    }
    int now = tail_index.back();
    std::vector<int> res;
    while(now != -1) {
        res.push_back(now);
        now = prev[now];
    }
    std::reverse(res.begin(), res.end());
    return res;
}

}//namespace kusu_lib