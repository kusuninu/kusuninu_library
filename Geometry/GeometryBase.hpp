#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {

enum Position { OUTSIDE = 0, ON = 1, INSIDE = 2 };

enum SegmentRelation {
    DISJOINT,           /*   / \   */ 
    INTERSECT,          //   X
    TOUCH,              //   <
    OVERLAP             //   --==--
    //COLLINEAR_DISJOINT  //   ---  ---
};

template<class T> constexpr T eps() {
    if constexpr(std::is_integral_v<T>) return static_cast<T>(0);
    if constexpr(std::is_floating_point_v<T>) {
        if constexpr(std::same_as<T, double>) return static_cast<double>(1e-9);
        if constexpr(std::same_as<T, long double>) return static_cast<long double>(1e-12);
        return static_cast<T>(1e-9);
    }
    return static_cast<T>(0);
}

template<class T> int sign(T x) {
    if(x > eps<T>()) return 1;
    if(x < -eps<T>()) return -1;
    return 0;
}

}//namespace kusu_lib