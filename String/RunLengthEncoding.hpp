#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<class T> std::vector<std::pair<T, int>> RunLengthEncoding(const std::vector<T>& V) {
    if(V.empty()) return std::vector<std::pair<T, int>>(0);
    int c = 1;
    std::vector<std::pair<T, int>> res;
    for(int i = 1; i < (int)V.size(); i++) {
        if(V[i - 1] == V[i]) c++;
        else {
            res.emplace_back(V[i - 1], c);
            c = 1;
        }
    }
    res.emplace_back(V.back(), c);
    return res;
}

std::vector<std::pair<char, int>> RunLengthEncoding(const std::string& V) {
    if(V.empty()) return std::vector<std::pair<char, int>>(0);
    int c = 1;
    std::vector<std::pair<char, int>> res;
    for(int i = 1; i < (int)V.size(); i++) {
        if(V[i - 1] == V[i]) c++;
        else {
            res.emplace_back(V[i - 1], c);
            c = 1;
        }
    }
    res.emplace_back(V.back(), c);
    return res;
}
}//namespace kusu_lib