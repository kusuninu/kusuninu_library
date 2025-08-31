#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<class T> struct StaticPolynomial {
  private:
    int _N;
    T* V;

  public:
    StaticPolynomial(int N) : _N(N) {
        V = new T[_N];
        memset(V, 0, sizeof(T) * _N);
    }

    StaticPolynomial(std::vector<T> v) : _N(v.size()) {
        V = new T[_N];
        for(int i = 0; i < _N; i++) V[i] = v[i];
    }

    void set(int p, T x) { V[p] = x; };

    T get(int p) { return V[p]; }

    int size() { return _N; }

    StaticPolynomial<T> operator+(const StaticPolynomial<T>& A) const {
        assert(_N == A._N);
        StaticPolynomial<T> res(_N);
        for(int i = 0; i < _N; i++) res.V[i] = V[i] + A.V[i];
        return res;
    }

    StaticPolynomial<T>& operator+=(const StaticPolynomial<T>& A) {
        assert(_N == A._N);
        for(int i = 0; i < _N; i++) V[i] += A.V[i];
        return *this;
    }

    StaticPolynomial<T> operator-(const StaticPolynomial<T>& A) const {
        assert(_N == A._N);
        StaticPolynomial<T> res(_N);
        for(int i = 0; i < _N; i++) res.V[i] = V[i] - A.V[i];
        return res;
    }

    StaticPolynomial<T>& operator-=(const StaticPolynomial<T>& A) {
        assert(_N == A._N);
        for(int i = 0; i < _N; i++) V[i] -= A.V[i];
        return *this;
    }

    StaticPolynomial<T> operator*(const StaticPolynomial<T>& A) const {
        assert(_N == A._N);
        StaticPolynomial<T> res(_N);
        for(int i = 0; i < _N; i++) {
            for(int j = 0; j < A._N - i; j++) res.V[i + j] += V[i] * A.V[j];
        }
        return res;
    }

    StaticPolynomial<T>& operator*=(const StaticPolynomial<T>& A) {
        memset(V, 0, sizeof(V));
        for(int i = 0; i < _N; i++) {
            for(int j = 0; j < A._N - i; j++) V[i + j] += V[i] * A.V[j];
        }
        return *this;
    }

    void print() const {
        for(int i = 0; i < _N; i++) std::cout << V[i] << " \n"[i == _N - 1];
    }
};
}//namespace kusu_lib