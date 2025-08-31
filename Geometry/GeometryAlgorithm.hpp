#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<class T> struct Point {
    T x;
    T y;
    Point(T x = 0, T y = 0) : x(x), y(y) {}

    template<class U, class V> Point(const std::pair<U, V>& p) : x(p.first), y(p.second) {}

    template<class U> Point(const Point<U>& p) : x(p.x), y(p.y) {}

    long double abs() { return sqrtl(x * x + y * y); }

    T square_abs() { return x * x + y * y; }

    long double arg() { return std::atan2(y, x); }

    Point<T> operator+() const { return *this; }

    Point<T> operator-() const { return Point<T>(-x, -y); }

    template<class U> Point<T>& operator+=(const Point<U>& e) {
        x += e.x;
        y += e.y;
        return *this;
    }

    template<class U> Point<T>& operator-=(const Point<U>& e) {
        x -= e.x;
        y -= e.y;
        return *this;
    }

    template<class U> Point<T>& operator*=(const U& e) {
        x *= e;
        y *= e;
        return *this;
    }
};

template<class U, class V> bool operator<(const Point<U>& lhs, const Point<V>& rhs) {
    if(lhs.x != rhs.x) return lhs.x < rhs.x;
    return lhs.y < rhs.y;
}

template<class U, class V> bool operator<=(const Point<U>& lhs, const Point<V>& rhs) {
    return !(rhs < lhs);
}

template<class U, class V> bool operator>(const Point<U>& lhs, const Point<V>& rhs) {
    return rhs < lhs;
}

template<class U, class V> bool operator>=(const Point<U>& lhs, const Point<V>& rhs) {
    return !(lhs < rhs);
}

template<class U, class V> bool operator==(const Point<U>& lhs, const Point<V>& rhs) {
    return lhs.x = rhs.x && lhs.y == rhs.y;
}

template<class U, class V> bool operator!=(const Point<U>& lhs, const Point<V>& rhs) {
    return !(lhs == rhs);
}

template<class U, class V>
Point<typename std::common_type<U, V>::type> operator+(const Point<U>& lhs, const Point<V>& rhs) {
    return Point<typename std::common_type<U, V>::type>(lhs) += rhs;
}

template<class U, class V>
Point<typename std::common_type<U, V>::type> operator-(const Point<U>& lhs, const Point<V>& rhs) {
    return Point<typename std::common_type<U, V>::type>(lhs) -= rhs;
}

template<class U, class V>
typename std::common_type<U, V>::type dot(const Point<U>& lhs, const Point<V>& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

template<class U, class V>
typename std::common_type<U, V>::type cross(const Point<U>& lhs, const Point<V>& rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

template<class U, class V> long double dist(const Point<U>& lhs, const Point<V>& rhs) {
    return sqrtl((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y));
}

template<class U, class V>
typename std::common_type<U, V>::type square_dist(const Point<U>& lhs, const Point<V>& rhs) {
    return (lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y);
}

template<class U> std::istream& operator>>(std::istream& is, Point<U>& f) {
    return is >> f.x >> f.y;
}

template<class U> std::ostream& operator<<(std::ostream& os, const Point<U>& f) {
    return os << f.x << " " << f.y;
}

//result is written in res
template<class T> int Convex_Hull(std::vector<Point<T>> V, std::vector<Point<T>>& res) {
    int N = V.size();
    if(N <= 2) {
        res = V;
        if(N <= 1) return 0;
        return 1;
    }
    std::sort(V.begin(), V.end());
    res = std::vector<Point<T>>(2 * N);
    int k = 0;
    for(int i = 0; i < N; i++) {
        while(2 <= k && cross(res[k - 1] - res[k - 2], V[i] - res[k - 1]) <= 0) k--;
        res[k] = V[i];
        k++;
    }
    int d = k - 1;
    for(int i = N - 2; i >= 0; i--) {
        while(d + 2 <= k && cross(res[k - 1] - res[k - 2], V[i] - res[k - 1]) <= 0) k--;
        res[k] = V[i];
        k++;
    }
    res.resize(k - 1);
    return d;
}
}//namespace kusu_lib