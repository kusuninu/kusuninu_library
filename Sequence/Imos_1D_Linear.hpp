#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<class T = long long> struct Imos_1D_Linear {
    Imos_1D_Linear(int N) : _N(N) {
        assert(0 < _N);
        A = new T[_N];
        for(int i = 0; i < _N; i++) A[i] = 0;
        B = new T[_N];
        for(int i = 0; i < _N; i++) B[i] = 0;
        C = new T[_N];
        for(int i = 0; i < _N; i++) C[i] = 0;
    }
    Imos_1D_Linear(const std::vector<T>& V) : _N(V.size()) {
        assert(0 < _N);
        A = new T[_N];
        for(int i = 0; i < _N; i++) A[i] = V[i];
        B = new T[_N];
        B[0] = A[0];
        for(int i = 0; i < _N - 1; i++) B[i + 1] = A[i + 1] - A[i];
        C = new T[_N];
        C[0] = B[0];
        for(int i = 0; i < _N - 1; i++) C[i + 1] = B[i + 1] - B[i];
    }
    //add a*(x-l)+b to [l,r)
    void add(int l, int r, T a, T b) {
        assert(0 <= l && l <= r && r <= _N);
        if(l != _N) C[l] += b;
        if(l + 1 < _N) C[l + 1] += a - b;
        if(r != _N) C[r] += -(r - l) * a - b;
        if(r + 1 < _N) C[r + 1] += (r - l - 1) * a + b;
        initialized = false;
    }
    T get(int p) {
        assert(0 <= p && p < _N);
        if(initialized) return A[p];
        B[0] = C[0];
        for(int i = 0; i < _N - 1; i++) B[i + 1] = B[i] + C[i + 1];
        A[0] = B[0];
        for(int i = 0; i < _N - 1; i++) A[i + 1] = A[i] + B[i + 1];
        initialized = true;
        return A[p];
    }

  private:
    T* A;
    T* B;
    T* C;
    int _N;
    bool initialized = true;
};
}//namespace kusu_lib