#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {

template<class T>
std::vector<T> xor_basis(std::span<const T> A) {
    std::vector<T> basis;
    for(T a: A) {
        for(T b: basis)
            if(a ^ b < a) a ^= b;

        if(a != 0) {
            for(T& b: basis)
                if(b ^ a < b) b ^= a;
            basis.push_back(a);
        }
    }

    std::ranges::sort(basis);
    return basis;
}

}//namespace kusu_lib