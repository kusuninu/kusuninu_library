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

template<class T, std::size_t N, std::size_t M>
struct Matrix {
  private:
    std::array<T, N * M> A;

    

    template<std::size_t n, std::size_t m, std::size_t l>
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

    template<std::convertible_to<T> U>
    Matrix<T, N, M>& operator+=(const Matrix<U, N, M>& B) {
        for(int i = 0; i < (int)(N * M); i++) A[i] += B.A[i];
        return *this;
    }

    template<std::convertible_to<T> U>
    Matrix<T, N, M>& operator-=(const Matrix<U, N, M>& B) {
        for(int i = 0; i < (int)(N * M); i++) A[i] -= B.A[i];
        return *this;
    }

    template<std::convertible_to<T> U>
    Matrix<T, N, M>& operator*=(const U& v) {
        for(int i = 0; i < (int)(N * M); i++) A[i] *= v;
        return *this;
    }

    template<std::size_t W>
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
    template<class U, std::size_t N2, std::size_t M2>
    friend struct Matrix;
};

template<class T, std::size_t N, std::size_t M>
Matrix<T, N, M> operator+(const Matrix<T, N, M>& lhs, const Matrix<T, N, M>& rhs) {
    return Matrix<T, N, M>(lhs) += rhs;
}

template<class T, std::size_t N, std::size_t M>
Matrix<T, N, M> operator-(const Matrix<T, N, M>& lhs, const Matrix<T, N, M>& rhs) {
    return Matrix<T, N, M>(lhs) -= rhs;
}

template<class T, std::size_t N, std::size_t M>
Matrix<T, N, M> operator*(const Matrix<T, N, M>& lhs, const T& rhs) {
    return Matrix<T, N, M>(lhs) *= rhs;
}

template<class T, std::size_t N, std::size_t M>
Matrix<T, N, M> operator*(const T& lhs, const Matrix<T, N, M>& rhs) {
    return Matrix<T, N, M>(rhs) *= lhs;
}

template<class U, std::size_t N, std::size_t M>
std::ostream& operator<<(std::ostream& os, const Matrix<U, N, M>& A) {
    for(int i = 0; i < (int)(N * M); i++) os << A.get(i / M, i % M) << ((i + 1) % M ? " " : "\n");
    return os;
}

}// namespace kusu_lib