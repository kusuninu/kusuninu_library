#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {

template<class T>
concept HasZeroOne = requires {
    { T::zero() } -> std::same_as<T>;
    { T::one() } -> std::same_as<T>;
};

template<class T>
struct IdentityProvider {
    static T zero() {
        if constexpr(HasZeroOne<T>) return T::zero();
        else return T();
    }
    static T one() {
        if constexpr(HasZeroOne<T>) return T::one();
        else return T(1);
    }
};

// =====================
// Matrix
// =====================
template<class T, int N = -1, int M = -1>
struct Matrix {
  private:
    int _N = 0;
    int _M = 0;
    std::vector<T> A;

    static void fast_mul(int n, int m, int l, const std::vector<T>& X, const std::vector<T>& Y,
                         std::vector<T>& Z) {
        T zero = IdentityProvider<T>::zero();
        for(int i = 0; i < n * l; i++) Z[i] = zero;
        for(int i = 0; i < n; i++) {
            for(int k = 0; k < m; k++) {
                for(int j = 0; j < l; j++) { Z[i * l + j] = Z[i * l + j] + X[i * m + k] * Y[k * l + j]; }
            }
        }
    }

    struct Row {
        T* row;
        int width;
        Row(T* r, int w) : row(r), width(w) {}
        T& operator[](int j) {
            assert(0 <= j && j < width);
            return row[j];
        }
        const T& operator[](int j) const {
            assert(0 <= j && j < width);
            return row[j];
        }
    };

  public:
    Matrix() = default;

    Matrix(int _N, int _M) : Matrix(_N, _M, IdentityProvider<T>::zero()) {}

    Matrix(int _N, int _M, T c) : _N(_N), _M(_M), A(_N * _M, c) {}

    Matrix(const std::vector<std::vector<T>>& V) {
        _N = V.size();
        _M = (_N ? V[0].size() : 0);
        A.reserve(_N * _M);
        for(const auto& row: V) {
            assert((int)row.size() == _M);
            for(const auto& x: row) A.push_back(x);
        }
    }

    Matrix(int _N, int _M, const std::vector<T>& V) : _N(_N), _M(_M), A(V) {
        assert((int)V.size() == _N * _M);
    }

    int height() const { return _N; }
    int width() const { return _M; }

    Row operator[](int i) {
        assert(0 <= i && i < _N);
        return Row(&A[i * _M], _M);
    }
    const Row operator[](int i) const {
        assert(0 <= i && i < _N);
        return Row(const_cast<T*>(&A[i * _M]), _M);
    }

    void set(int i, int j, T t) { A[i * _M + j] = t; }

    T get(int i, int j) const { return A[i * _M + j]; }

    static Matrix Identity_Matrix(int n) {
        Matrix B(n, n, IdentityProvider<T>::zero());
        for(int i = 0; i < n; i++) B.A[i * n + i] = IdentityProvider<T>::one();
        return B;
    }

    static Matrix Zero_Matrix(int n) { return Matrix(n, n, IdentityProvider<T>::zero()); }

    void clear_row(int i) {
        assert(0 <= i && i < _N);
        for(int j = 0; j < _M; j++) A[i * _M + j] = IdentityProvider<T>::zero();
    }

    void clear_column(int i) {
        assert(0 <= i && i < _M);
        for(int j = 0; j < _N; j++) A[j * _M + i] = IdentityProvider<T>::zero();
    }

    Matrix<T>& operator+=(const Matrix<T>& B) {
        assert(_N == B._N && _M == B._M);
        for(int i = 0; i < _N * _M; i++) A[i] += B.A[i];
        return *this;
    }

    Matrix<T>& operator-=(const Matrix<T>& B) {
        assert(_N == B._N && _M == B._M);
        for(int i = 0; i < _N * _M; i++) A[i] -= B.A[i];
        return *this;
    }

    Matrix<T>& operator*=(const T& v) {
        for(int i = 0; i < _N * _M; i++) A[i] *= v;
        return *this;
    }

    Matrix<T> operator*(const Matrix<T>& B) const {
        assert(_M == B.height());
        int W = B.width();
        std::vector<T> C(_N * W);
        fast_mul(_N, _M, W, A, B.A, C);
        return Matrix<T>(_N, W, C);
    }

    Matrix<T> pow(long long n) const {
        assert(0 <= n);
        assert(_N == _M);
        std::vector<T> temp1(_N * _N), temp2(_N * _N), temp3(_N * _N), temp4(_N * _N);
        int k = 0;
        int l = 0;
        temp3 = A;
        temp1.assign(_N * _N, IdentityProvider<T>::zero());
        for(int i = 0; i < _N; i++) temp1[i * _N + i] = IdentityProvider<T>::one();
        while(n) {
            if(n & 1) {
                if(k) {
                    if(l) fast_mul(_N, _N, _N, temp2, temp4, temp1);
                    else fast_mul(_N, _N, _N, temp1, temp4, temp2);
                }
                else {
                    if(l) fast_mul(_N, _N, _N, temp2, temp3, temp1);
                    else fast_mul(_N, _N, _N, temp1, temp3, temp2);
                }
                l ^= 1;
            }

            if(k) fast_mul(_N, _N, _N, temp4, temp4, temp3);
            else fast_mul(_N, _N, _N, temp3, temp3, temp4);
            n >>= 1;
            k ^= 1;
        }
        if(l) { return Matrix<T>(_N, _N, temp2); }
        else { return Matrix<T>(_N, _N, temp1); }
    }

    Matrix<T> Transpose() const {
        Matrix<T> B(_M, _N, IdentityProvider<T>::zero());
        for(int i = 0; i < _N; i++) {
            for(int j = 0; j < _M; j++) { B.set(j, i, A[i * _M + j]); }
        }
        return B;
    }
};

template<class T>
Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    return Matrix<T>(lhs) += rhs;
}

template<class T>
Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    return Matrix<T>(lhs) -= rhs;
}

template<class T>
Matrix<T> operator*(const Matrix<T>& lhs, const T& rhs) {
    return Matrix<T>(lhs) *= rhs;
}

template<class T>
Matrix<T> operator*(const T& lhs, const Matrix<T>& rhs) {
    return Matrix<T>(rhs) *= lhs;
}
template<class T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& A) {
    for(int i = 0; i < A.height() * A.width(); i++)
        os << A.get(i / A.width(), i % A.width()) << ((i + 1) % A.width() ? " " : "\n");
    return os;
}

template<class T, int N, int M>
requires(N != -1 && M != -1)
struct Matrix<T, N, M> {
  private:
    std::array<T, N * M> A;

    template<int n, int m, int l>
    static void fast_mul(const std::array<T, n * m>& X, const std::array<T, m * l>& Y,
                         std::array<T, n * l>& Z) {
        T zero = IdentityProvider<T>::zero();
        for(int i = 0; i < (int)(n * l); i++) Z[i] = zero;
        for(int i = 0; i < (int)n; i++) {
            for(int k = 0; k < (int)m; k++) {
                for(int j = 0; j < (int)l; j++) { Z[i * l + j] = Z[i * l + j] + X[i * m + k] * Y[k * l + j]; }
            }
        }
    }

    struct Row {
        T* row;
        int width;
        Row(T* r, int w) : row(r), width(w) {}
        T& operator[](int j) {
            assert(0 <= j && j < width);
            return row[j];
        }
        const T& operator[](int j) const {
            assert(0 <= j && j < width);
            return row[j];
        }
    };

  public:
    Matrix() { A.fill(IdentityProvider<T>::zero()); }

    Matrix(T c) { A.fill(c); }

    Matrix(const std::vector<std::vector<T>>& V) {
        assert(V.size() == N);
        for(int i = 0; i < (int)N; i++) {
            assert(V[i].size() == M);
            for(int j = 0; j < (int)M; j++) A[i * M + j] = V[i][j];
        }
    }

    Matrix(const std::array<T, N * M>& V) : A(V) {}

    static constexpr int height() { return (int)N; }
    static constexpr int width() { return (int)M; }

    Row operator[](int i) {
        assert(0 <= i && i < (int)N);
        return Row(&A[i * M], (int)M);
    }
    const Row operator[](int i) const {
        assert(0 <= i && i < (int)N);
        return Row(const_cast<T*>(&A[i * M]), (int)M);
    }

    void set(int i, int j, T t) { A[i * M + j] = t; }

    T get(int i, int j) const { return A[i * M + j]; }

    static Matrix Identity_Matrix() {
        static_assert(N == M);
        Matrix B(IdentityProvider<T>::zero());
        for(int i = 0; i < (int)N; i++) B.A[i * N + i] = IdentityProvider<T>::one();
        return B;
    }

    static Matrix Zero_Matrix() { return Matrix(IdentityProvider<T>::zero()); }

    void clear_row(int i) {
        assert(0 <= i && i < (int)N);
        for(int j = 0; j < (int)M; j++) A[i * M + j] = IdentityProvider<T>::zero();
    }

    void clear_column(int i) {
        assert(0 <= i && i < (int)M);
        for(int j = 0; j < (int)N; j++) A[j * M + i] = IdentityProvider<T>::zero();
    }

    Matrix<T, N, M>& operator+=(const Matrix<T, N, M>& B) {
        for(int i = 0; i < (int)(N * M); i++) A[i] += B.A[i];
        return *this;
    }

    Matrix<T, N, M>& operator-=(const Matrix<T, N, M>& B) {
        for(int i = 0; i < (int)(N * M); i++) A[i] -= B.A[i];
        return *this;
    }

    Matrix<T, N, M>& operator*=(const T& v) {
        for(int i = 0; i < (int)(N * M); i++) A[i] *= v;
        return *this;
    }

    template<int W>
    Matrix<T, N, W> operator*(const Matrix<T, M, W>& B) const {
        std::array<T, N * W> C;
        fast_mul<N, M, W>(A, B.A, C);
        return Matrix<T, N, W>(C);
    }

    Matrix<T, N, M> pow(long long n) const {
        static_assert(N == M);
        assert(0 <= n);
        static std::array<T, N * M> temp1, temp2, temp3, temp4;
        int k = 0;
        int l = 0;
        temp3 = A;
        temp1.fill(IdentityProvider<T>::zero());
        for(int i = 0; i < (int)N; i++) temp1[i * N + i] = IdentityProvider<T>::one();
        while(n) {
            if(n & 1) {
                if(k) {
                    if(l) fast_mul<N, N, N>(temp2, temp4, temp1);
                    else fast_mul<N, N, N>(temp1, temp4, temp2);
                }
                else {
                    if(l) fast_mul<N, N, N>(temp2, temp3, temp1);
                    else fast_mul<N, N, N>(temp1, temp3, temp2);
                }
                l ^= 1;
            }

            if(k) fast_mul<N, N, N>(temp4, temp4, temp3);
            else fast_mul<N, N, N>(temp3, temp3, temp4);
            n >>= 1;
            k ^= 1;
        }
        if(l) { return Matrix<T, N, M>(temp2); }
        else { return Matrix<T, N, M>(temp1); }
    }

    Matrix<T, M, N> Transpose() const {
        Matrix<T, M, N> B(IdentityProvider<T>::zero());
        for(int i = 0; i < (int)N; i++) {
            for(int j = 0; j < (int)M; j++) { B.set(j, i, A[i * M + j]); }
        }
        return B;
    }

    // operator+, -, * need access to internal A
    template<class U, int N2, int M2>
    friend struct Matrix;
};

template<class T, int N, int M>
requires(N != -1 && M != -1)
Matrix<T, N, M> operator+(const Matrix<T, N, M>& lhs, const Matrix<T, N, M>& rhs) {
    return Matrix<T, N, M>(lhs) += rhs;
}

template<class T, int N, int M>
requires(N != -1 && M != -1)
Matrix<T, N, M> operator-(const Matrix<T, N, M>& lhs, const Matrix<T, N, M>& rhs) {
    return Matrix<T, N, M>(lhs) -= rhs;
}

template<class T, int N, int M>
requires(N != -1 && M != -1)
Matrix<T, N, M> operator*(const Matrix<T, N, M>& lhs, const T& rhs) {
    return Matrix<T, N, M>(lhs) *= rhs;
}

template<class T, int N, int M>
requires(N != -1 && M != -1)
Matrix<T, N, M> operator*(const T& lhs, const Matrix<T, N, M>& rhs) {
    return Matrix<T, N, M>(rhs) *= lhs;
}

template<class T, int N, int M>
requires(N != -1 && M != -1)
std::ostream& operator<<(std::ostream& os, const Matrix<T, N, M>& A) {
    for(int i = 0; i < (int)(N * M); i++) os << A.get(i / M, i % M) << ((i + 1) % M ? " " : "\n");
    return os;
}

}//namespace kusu_lib