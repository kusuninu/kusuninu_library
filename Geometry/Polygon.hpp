#pragma once
#include <bits/stdc++.h>
#include "GeometryBase.hpp"
#include "Point.hpp"
#include "Segment.hpp"
namespace kusu_lib {
template<class T>
struct Polygon {
    std::vector<Point<T>> vertices;//Counterclockwise
    Polygon() {}
    //Counterclockwise
    Polygon(const std::vector<Point<T>>& vertices) : vertices(vertices) {}

    Point<T>& operator[](int i) {
        assert(0 <= i && i < vertices.size());
        return vertices[i];
    }

    const Point<T>& operator[](int i) const {
        assert(0 <= i && i < vertices.size());
        return vertices[i];
    }

    std::vector<Point<T>> get_vertices() const { return vertices; }

    int size() const { return vertices.size(); }

    Segment<T> edge(int i) const {
        assert(0 <= i && i < size());
        int ni = (i == size() - 1 ? 0 : i + 1);
        return Segment<T>(vertices[i], vertices[ni]);
    }

    bool is_convex() const {
        int N = (int)vertices.size();
        for(int i = 0; i < N; i++) {
            int j = i + 1;
            if(j == N) j = 0;
            int k = j + 1;
            if(k == N) k = 0;
            if(sign(cross(vertices[j] - vertices[i], vertices[k] - vertices[j])) == -1)
                return false;
        }
        return true;
    }
};

//@return signed area * 2
template<class T>
T area2(const Polygon<T>& polygon) {
    int N = (int)polygon.size();
    T res{};
    for(int i = 0; i < N; i++) {
        int j = i + 1;
        if(j == N) j = 0;
        res += cross(polygon[i], polygon[j]);
    }
    return res;
}

//@return signed area
template<class T>
T area(const Polygon<T>& polygon) {
    return area2(polygon) / 2;
}

//result is written in res
//d is size of bottom convex hull
template<class T>
std::pair<Polygon<T>, int> Convex_Hull(std::vector<Point<T>> V) {
    int N = V.size();
    std::vector<Point<T>> res;
    if(N <= 2) {
        res = V;
        if(N <= 1) return {Polygon<T>(res), 0};
        return {Polygon<T>(res), 1};
    }
    std::sort(V.begin(), V.end());
    res = std::vector<Point<T>>(2 * N);
    int k = 0;
    for(int i = 0; i < N; i++) {
        while(2 <= k && sign(cross(res[k - 1] - res[k - 2], V[i] - res[k - 1])) <= 0) k--;
        res[k] = V[i];
        k++;
    }
    int d = k - 1;
    for(int i = N - 2; i >= 0; i--) {
        while(d + 2 <= k && sign(cross(res[k - 1] - res[k - 2], V[i] - res[k - 1])) <= 0) k--;
        res[k] = V[i];
        k++;
    }
    res.resize(k - 1);
    return {Polygon<T>(res), d};
}

//polygon must be convex
template<class T>
std::pair<int, int> farthest_points(const Polygon<T>& polygon) {
    if(polygon.size() == 0) return {};
    if(polygon.size() == 1) return {0, 0};
    if(polygon.size() == 2) return {0, 1};
    int si = 0;
    int sj = 0;
    for(int k = 0; k < polygon.size(); k++) {
        if(polygon[si] < polygon[k]) si = k;
        if(polygon[sj] > polygon[k]) sj = k;
    }
    int i = si;
    int j = sj;
    T res = 0;
    std::pair<int, int> ans;
    while(i != sj || j != si) {
        T d = dist2(polygon[i], polygon[j]);
        if(res < d) {
            res = d;
            ans = {i, j};
        }
        int ni = (i + 1 == polygon.size() ? 0 : i + 1);
        int nj = (j + 1 == polygon.size() ? 0 : j + 1);
        if(sign(cross(polygon[ni] - polygon[i], polygon[nj] - polygon[j])) == -1) i = ni;
        else j = nj;
    }
    return ans;
}

}//namespace kusu_lib