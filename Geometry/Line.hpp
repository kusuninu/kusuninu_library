#pragma once
#include <bits/stdc++.h>
#include "GeometryBase.hpp"
#include "Point.hpp"
namespace kusu_lib {
template<class T>
struct Line {
    Point<T> a;
    Point<T> b;
    Line() {}
    Line(const Point<T>& a, const Point<T>& b) : a(a), b(b) {}
};

template<class T>
Point<T> dir(const Line<T>& line) {
    return line.b - line.a;
}

template<class T>
T length2(const Line<T>& line) {
    Point<T> v = dir(line);
    return norm2(v);
}

template<class T>
T length(const Line<T>& line) {
    return static_cast<T>(sqrtl(length2(line)));
}

};//namespace kusu_lib