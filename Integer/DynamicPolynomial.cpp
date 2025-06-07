#include <bits/stdc++.h>
template<class T> struct DynamicPolynomial {
  private:
    int _N;
    std::vector<T> V;

  public:
    DynamicPolynomial(int N) : _N(N) { V.resize(_N); }

    void set(int p, T x) { V[p] = x; };

    T get(int p) { return V[p]; }

    int size() { return _N; }

    DynamicPolynomial<T> operator+(const DynamicPolynomial<T>& A) const {
        int M = std::max(_N, A._N);
        DynamicPolynomial<T> res(M);
        int m = std::min(_N, A._N);
        for(int i = 0; i < m; i++) res.V[i] = V[i] + A.V[i];
        if(_N < A._N) {
            for(int i = m; i < M; i++) res.V[i] = A.V[i];
        }
        if(A._N < _N) {
            for(int i = m; i < M; i++) res.V[i] = V[i];
        }
        return res;
    }

    DynamicPolynomial<T>& operator+=(const DynamicPolynomial<T>& A) {
        int M = std::max(_N, A._N);
        int m = std::min(_N, A._N);
        for(int i = 0; i < m; i++) V[i] = V[i] + A.V[i];
        if(_N < A._N) {
            for(int i = m; i < M; i++) V[i] = A.V[i];
        }
        if(A._N < _N) {
            for(int i = m; i < M; i++) V[i] = V[i];
        }
        return *this;
    }

    DynamicPolynomial<T> operator-(const DynamicPolynomial<T>& A) const {
        int M = std::max(_N, A._N);
        DynamicPolynomial<T> res(M);
        int m = std::min(_N, A._N);
        for(int i = 0; i < m; i++) res.V[i] = V[i] - A.V[i];
        if(_N < A._N) {
            for(int i = m; i < M; i++) res.V[i] = A.V[i];
        }
        if(A._N < _N) {
            for(int i = m; i < M; i++) res.V[i] = V[i];
        }
        return res;
    }

    DynamicPolynomial<T>& operator-=(const DynamicPolynomial<T>& A) {
        int M = std::max(_N, A._N);
        int m = std::min(_N, A._N);
        for(int i = 0; i < m; i++) V[i] = V[i] - A.V[i];
        if(_N < A._N) {
            for(int i = m; i < M; i++) V[i] = A.V[i];
        }
        if(A._N < _N) {
            for(int i = m; i < M; i++) V[i] = V[i];
        }
        return *this;
    }

    DynamicPolynomial<T> operator*(const DynamicPolynomial<T>& A) const {
        int M = _N + A._N - 1;
        DynamicPolynomial<T> res(M);
        for(int i = 0;i < _N;i++){
            for(int j = 0;j < A._N;j++){
                res.V[i+j] += V[i] * A.V[j];
            }
        } 
        return res;
    }
    DynamicPolynomial<T>& operator*=(const DynamicPolynomial<T>& A) {
        int M = _N + A._N - 1;
        V.resize(M);
        V.assign(M,0);
        for(int i = 0;i < _N;i++){
            for(int j = 0;j < A._N;j++){
                V[i+j] += V[i] * A.V[j];
            }
        }
        return *this;
    }

    void print() const {
        for(int i = 0; i < _N; i++) cout << V[i] << " \n"[i == _N - 1];
    }
};