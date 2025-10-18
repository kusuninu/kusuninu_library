#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<class T>
struct Point {
    T x;
    T y;
    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    template<class U, class V>
    Point(const std::pair<U, V>& p) : x(p.first), y(p.second) {}

    template<class U>
    Point(const Point<U>& p) : x(p.x), y(p.y) {}

    Point<T> operator+() const { return *this; }

    Point<T> operator-() const { return Point<T>(-x, -y); }

    template<class U>
    Point<T>& operator+=(const Point<U>& e) {
        x += e.x;
        y += e.y;
        return *this;
    }

    template<class U>
    Point<T>& operator-=(const Point<U>& e) {
        x -= e.x;
        y -= e.y;
        return *this;
    }

    template<class U>
    Point<T>& operator*=(const U& e) {
        x *= e;
        y *= e;
        return *this;
    }

    template<class U>
    Point<T>& operator/=(const U& e) {
        x /= e;
        y /= e;
        return *this;
    }
};

template<class U, class V>
bool operator<(const Point<U>& lhs, const Point<V>& rhs) {
    if(lhs.x != rhs.x) return lhs.x < rhs.x;
    return lhs.y < rhs.y;
}

template<class U, class V>
bool operator<=(const Point<U>& lhs, const Point<V>& rhs) {
    return !(rhs < lhs);
}

template<class U, class V>
bool operator>(const Point<U>& lhs, const Point<V>& rhs) {
    return rhs < lhs;
}

template<class U, class V>
bool operator>=(const Point<U>& lhs, const Point<V>& rhs) {
    return !(lhs < rhs);
}

template<class U, class V>
bool operator==(const Point<U>& lhs, const Point<V>& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template<class U, class V>
bool operator!=(const Point<U>& lhs, const Point<V>& rhs) {
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
Point<typename std::common_type<U, V>::type> operator*(const Point<U>& lhs, const V& rhs) {
    return Point<typename std::common_type<U, V>::type>(lhs) *= rhs;
}

template<class U, class V>
Point<typename std::common_type<U, V>::type> operator*(const U& lhs, const Point<V>& rhs) {
    return Point<typename std::common_type<U, V>::type>(rhs) *= lhs;
}

template<class U, class V>
Point<typename std::common_type<U, V>::type> operator/(const Point<U>& lhs, const V& rhs) {
    return Point<typename std::common_type<U, V>::type>(lhs) /= rhs;
}

template<class T>
T norm(const Point<T>& p) {
    return static_cast<T>(sqrtl(norm2(p)));
}

template<class T>
T norm2(const Point<T>& p) {
    return p.x * p.x + p.y * p.y;
}

template<class T>
long double arg(const Point<T>& p) {
    return std::atan2(p.y, p.x);
}

template<class U, class V>
typename std::common_type<U, V>::type dot(const Point<U>& lhs, const Point<V>& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

template<class U, class V>
typename std::common_type<U, V>::type cross(const Point<U>& lhs, const Point<V>& rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

template<class U, class V>
long double dist(const Point<U>& lhs, const Point<V>& rhs) {
    return sqrtl(dist2(lhs, rhs));
}

template<class U, class V>
typename std::common_type<U, V>::type dist2(const Point<U>& lhs, const Point<V>& rhs) {
    return (lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y);
}

template<class U>
std::istream& operator>>(std::istream& is, Point<U>& f) {
    return is >> f.x >> f.y;
}

template<class U>
std::ostream& operator<<(std::ostream& os, const Point<U>& f) {
    return os << f.x << " " << f.y;
}

}//namespace kusu_lib