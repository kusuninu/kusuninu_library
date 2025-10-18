#pragma once
#include <bits/stdc++.h>
#include "GeometryBase.hpp"
#include "Line.hpp"
#include "Point.hpp"
#include "Polygon.hpp"
#include "Segment.hpp"

namespace kusu_lib {

template<class T>
bool is_collinear(const Point<T>& a, const Point<T>& b, const Point<T>& c) {
    return sign(cross(b - a, c - a)) == 0;
}

template<class T>
bool is_collinear(const Point<T>& a, const Point<T>& b, const Point<T>& c, const Point<T>& d) {
    return sign(cross(b - a, c - a)) == 0 && sign(cross(c - a, d - a)) == 0;
}

template<class T>
bool is_equal(const Line<T>& l, const Line<T>& m) {
    return is_collinear(l.a, l.b, m.a, m.b);
}

template<class T>
bool is_parallel(const Line<T>& l, const Line<T>& m) {
    return sign(cross(dir(l), dir(m))) == 0;
}

template<class T>
bool is_parallel(const Line<T>& l, const Segment<T>& m) {
    return sign(cross(dir(l), dir(m))) == 0;
}

template<class T>
bool is_parallel(const Segment<T>& l, const Line<T>& m) {
    return sign(cross(dir(l), dir(m))) == 0;
}

template<class T>
bool is_parallel(const Segment<T>& l, const Segment<T>& m) {
    return sign(cross(dir(l), dir(m))) == 0;
}

template<class T>
bool is_orthogonal(const Line<T>& l, const Line<T>& m) {
    return sign(dot(dir(l), dir(m))) == 0;
}

template<class T>
bool is_orthogonal(const Line<T>& l, const Segment<T>& m) {
    return sign(dot(dir(l), dir(m))) == 0;
}

template<class T>
bool is_orthogonal(const Segment<T>& l, const Line<T>& m) {
    return sign(dot(dir(l), dir(m))) == 0;
}

template<class T>
bool is_orthogonal(const Segment<T>& l, const Segment<T>& m) {
    return sign(dot(dir(l), dir(m))) == 0;
}

template<class T>
bool is_on_line(const Point<T>& p, const Line<T>& l) {
    return is_collinear(p, l.a, l.b);
}

template<class T>
bool is_on_segment(const Point<T>& p, const Segment<T>& s) {
    return is_collinear(p, s.a, s.b) && sign(dot(p - s.a, p - s.b)) <= 0;
}

template<class T>
SegmentRelation has_intersect(const Segment<T>& s1, const Segment<T>& s2) {
    Point<T> d1 = dir(s1), d2 = dir(s2);
    T c1 = cross(d1, s2.a - s1.a);
    T c2 = cross(d1, s2.b - s1.a);
    T c3 = cross(d2, s1.a - s2.a);
    T c4 = cross(d2, s1.b - s2.a);
    if(sign(c1) == 0 && sign(c2) == 0) {
        if(sign(std::min(s2.a.x, s2.b.x) - std::max(s1.a.x, s1.b.x)) == 1) return DISJOINT;
        if(sign(std::min(s1.a.x, s1.b.x) - std::max(s2.a.x, s2.b.x)) == 1) return DISJOINT;
        if(sign(std::min(s2.a.y, s2.b.y) - std::max(s1.a.y, s1.b.y)) == 1) return DISJOINT;
        if(sign(std::min(s1.a.y, s1.b.y) - std::max(s2.a.y, s2.b.y)) == 1) return DISJOINT;
        return OVERLAP;
    }
    int sa = sign(c1) * sign(c2);
    int sb = sign(c3) * sign(c4);
    if(sa * sb == 0) return TOUCH;
    if(sa == -1 && sb == -1) return INTERSECT;
    return DISJOINT;
}

template<class T>
SegmentRelation has_intersect(const Line<T>& l1, const Segment<T>& s2) {
    Point<T> d1 = dir(l1), d2 = dir(s2);
    T c1 = cross(d1, s2.a - l1.a);
    T c2 = cross(d1, s2.b - l1.a);
    if(sign(c1) == 0 && sign(c2) == 0) return OVERLAP;
    int s = sign(c1) * sign(c2);
    if(s == 1) return DISJOINT;
    if(s == 0) return TOUCH;
    return INTERSECT;
}

template<class T>
SegmentRelation has_intersect(const Segment<T>& s1, const Line<T>& l2) {
    return has_intersect(l2, s1);
}

template<class T>
SegmentRelation has_intersect(const Line<T>& l1, const Line<T>& l2) {
    if(is_equal(l1, l2)) return OVERLAP;
    if(parallel(l1, l2)) return DISJOINT;
    return INTERSECT;
}

template<class T>
std::optional<Point<T>> intersect(const Line<T>& l1, const Line<T>& l2) {
    auto f = has_intersect(l1, l2);
    if(f != INTERSECT && f != TOUCH) return std::nullopt;
    Point<T> d1 = dir(l1), d2 = dir(l2);
    T t = cross(l2.a - l1.a, d2) / cross(d1, d2);
    return l1.a + t * d1;
}

template<class T>
std::optional<Point<T>> intersect(const Line<T>& l1, const Segment<T>& s2) {
    auto f = has_intersect(l1, s2);
    if(f != INTERSECT && f != TOUCH) return std::nullopt;
    Point<T> d1 = dir(l1), d2 = dir(s2);
    T t = cross(s2.a - l1.a, d2) / cross(d1, d2);
    return l1.a + t * d1;
}

template<class T>
std::optional<Point<T>> intersect(const Segment<T>& s1, const Line<T>& l2) {
    return intersect(l2, s1);
}

template<class T>
std::optional<Point<T>> intersect(const Segment<T>& s1, const Segment<T>& s2) {
    auto f = has_intersect(s1, s2);
    if(f != INTERSECT && f != TOUCH) return std::nullopt;
    Point<T> d1 = dir(s1), d2 = dir(s2);
    T t = cross(s2.a - s1.a, d2) / cross(d1, d2);
    return s1.a + t * d1;
}

template<class T>
T dist2(const Point<T>& point, const Line<T>& line) {
    T c = cross(dir(line), point - line.a);
    return c * c / length2(line);
}

template<class T>
T dist(const Point<T>& point, const Line<T>& line) {
    T c = cross(dir(line), point - line.a);
    return c / length(line);
}

template<class T>
Point<T> project(const Point<T>& point, const Line<T>& line) {
    Point<T> d = dir(line);
    return line.a + dot(point - line.a, d) / norm2(d) * d;
}

template<class T>
Point<T> reflect(const Point<T>& point, const Line<T>& line) {
    return project(point, line) * 2 - point;
}

template<class T>
T dist2(const Point<T>& point, const Segment<T>& seg) {
    Point<T> d = dir(seg);
    if(sign(dot(d, point - seg.a)) == -1) return norm2(point - seg.a);
    if(sign(dot(-d, point - seg.b)) == -1) return norm2(point - seg.b);
    T c = cross(dir(seg), point - seg.a);
    return c * c / length2(seg);
}

template<class T>
T dist(const Point<T>& point, const Segment<T>& seg) {
    return static_cast<T>(sqrtl(dist2(point, seg)));
}

// O(polygon.size)
template<class T>
Position is_in_polygon(const Point<T>& point, const Polygon<T>& polygon) {
    if(polygon.size() == 0) return OUTSIDE;
    bool res = 0;
    T right = polygon[0].x;
    for(int i = 0; i < polygon.size(); i++) right = std::max(right, polygon[i].x);
    Segment<T> seg(point, Point<T>(right + 1, point.y));
    for(int i = 0; i < polygon.size(); i++) {
        Segment<T> e = polygon.edge(i);
        if(is_on_segment(point, e)) return ON;
        auto f = has_intersect(seg, e);
        if(f == TOUCH) {
            if(e.a.y < e.b.y) {
                if(is_on_segment(e.a, seg)) res = !res;
            }
            else {
                if(is_on_segment(e.b, seg)) res = !res;
            }
        }
        else if(f == INTERSECT) res = !res;
    }
    if(res) return INSIDE;
    return OUTSIDE;
}

// O(log(polygon.size))
template<class T>
Position is_in_convex_polygon(const Point<T>& point, const Polygon<T>& polygon) {}

//does not include seg.b
template<std::integral T>
T lattice_point_on_segment(const Segment<T>& seg) {
    T g = std::gcd(seg.b.x - seg.a.x, seg.b.y - seg.a.y);
    return g;
}

//does not include boundary
template<std::integral T>
T lattice_point_in_polygon(const Polygon<T>& polygon) {
    T S = std::abs(area2(polygon));
    T e = 0;
    for(int i = 0; i < polygon.size(); i++) e += lattice_point_on_segment(polygon.edge(i));
    return (S - e) / 2 + 1;
}

}//namespace kusu_lib