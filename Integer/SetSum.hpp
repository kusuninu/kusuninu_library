#pragma once
#include "StaticPolynomial.hpp"
#include <bits/stdc++.h>
namespace kusu_lib {
struct SetSum {
  public:
    template<class T> static void sum_over_subset(std::vector<T>& A) {
        int N = A.size();
        assert(std::__popcount(N) == 1);
        for(int i = 1; i < N; i <<= 1) {
            for(int j = 0; j < N; j++) {
                if(j & i) A[j] += A[j ^ i];
            }
        }
    }

    template<class T, class F> static void sum_over_subset(std::vector<T>& A, F f) {
        int N = A.size();
        assert(std::__popcount(N) == 1);
        for(int i = 1; i < N; i <<= 1) {
            for(int j = 0; j < N; j++) {
                if(j & i) A[j] = f(A[j], A[j ^ i]);
            }
        }
    }

    template<class T> static void inverse_sum_over_subset(std::vector<T>& A) {
        int N = A.size();
        assert(std::__popcount(N) == 1);
        for(int i = 1; i < N; i <<= 1) {
            for(int j = 0; j < N; j++) {
                if(j & i) A[j] -= A[j ^ i];
            }
        }
    }

    //f(a,b) = c <=> f_inv(c,b) = a
    template<class T, class F> static void inverse_sum_over_subset(std::vector<T>& A, F f_inv) {
        int N = A.size();
        assert(std::__popcount(N) == 1);
        for(int i = 1; i < N; i <<= 1) {
            for(int j = 0; j < N; j++) {
                if(j & i) A[j] = f_inv(A[j], A[j ^ i]);
            }
        }
    }

    template<class T> static void sum_over_superset(std::vector<T>& A) {
        int N = A.size();
        assert(std::__popcount(N) == 1);
        for(int i = 1; i < N; i <<= 1) {
            for(int j = 0; j < N; j++) {
                if(j & i) A[j ^ i] += A[j];
            }
        }
    }

    template<class T, class F> static void sum_over_superset(std::vector<T>& A, F f) {
        int N = A.size();
        assert(std::__popcount(N) == 1);
        for(int i = 1; i < N; i <<= 1) {
            for(int j = 0; j < N; j++) {
                if(j & i) A[j ^ i] = f(A[j ^ i], A[j]);
            }
        }
    }

    template<class T> static void inverse_sum_over_superset(std::vector<T>& A) {
        int N = A.size();
        assert(std::__popcount(N) == 1);
        for(int i = 1; i < N; i <<= 1) {
            for(int j = 0; j < N; j++) {
                if(j & i) A[j ^ i] -= A[j];
            }
        }
    }

    //f(a,b) = c <=> f_inv(c,b) = a
    template<class T, class F> static void inverse_sum_over_superset(std::vector<T>& A, F f_inv) {
        int N = A.size();
        assert(std::__popcount(N) == 1);
        for(int i = 1; i < N; i <<= 1) {
            for(int j = 0; j < N; j++) {
                if(j & i) A[j ^ i] = f_inv(A[j ^ i], A[j]);
            }
        }
    }

    template<class T>
    static std::vector<T> or_convolution(const std::vector<T>& A, const std::vector<T>& B) {
        assert(A.size() == B.size());
        int N = A.size();
        assert(std::__popcount(N) == 1);
        auto C_A = A;
        auto C_B = B;
        sum_over_subset(C_A);
        sum_over_subset(C_B);
        std::vector<T> C(N);
        for(int i = 0; i < N; i++) C[i] = C_A[i] * C_B[i];
        inverse_sum_over_subset(C);
        return C;
    }

    //sum(a,b) = c <=> sum_inv(c,b) = a
    //T,sum,mul must be a ring
    template<class T, T (*sum)(T, T), T (*sum_inv)(T, T), T (*mul)(T, T)>
    static std::vector<T> or_convolution(const std::vector<T>& A, const std::vector<T>& B) {
        assert(A.size() == B.size());
        int N = A.size();
        assert(std::__popcount(N) == 1);
        auto C_A = A;
        auto C_B = B;
        sum_over_subset(C_A, sum);
        sum_over_subset(C_B, sum);
        std::vector<T> C(N);
        for(int i = 0; i < N; i++) C[i] = mul(C_A[i], C_B[i]);
        inverse_sum_over_subset(C, sum_inv);
        return C;
    }

    template<class T>
    static std::vector<T> and_convolution(const std::vector<T>& A, const std::vector<T>& B) {
        assert(A.size() == B.size());
        int N = A.size();
        assert(std::__popcount(N) == 1);
        auto C_A = A;
        auto C_B = B;
        sum_over_superset(C_A);
        sum_over_superset(C_B);
        std::vector<T> C(N);
        for(int i = 0; i < N; i++) C[i] = C_A[i] * C_B[i];
        inverse_sum_over_superset(C);
        return C;
    }

    //sum(a,b) = c <=> sum_inv(c,b) = a
    //T,sum,mul must be a ring
    template<class T, T (*sum)(T, T), T (*sum_inv)(T, T), T (*mul)(T, T)>
    static std::vector<T> and_convolution(const std::vector<T>& A, const std::vector<T>& B) {
        assert(A.size() == B.size());
        int N = A.size();
        assert(std::__popcount(N) == 1);
        auto C_A = A;
        auto C_B = B;
        sum_over_superset(C_A, sum);
        sum_over_superset(C_B, sum);
        std::vector<T> C(N);
        for(int i = 0; i < N; i++) C[i] = mul(C_A[i], C_B[i]);
        inverse_sum_over_superset(C, sum_inv);
        return C;
    }

    template<class T>
    static std::vector<T> subset_convolution(const std::vector<T>& A, const std::vector<T>& B) {
        assert(A.size() == B.size());
        int N = A.size();
        assert(std::__popcount(N) == 1);
        int d = 31 - std::__countl_zero(N);
        std::vector<kusu_lib::StaticPolynomial<T>> C_A(N), C_B(N);
        for(int i = 0; i < N; i++) {
            C_A[i] = kusu_lib::StaticPolynomial<T>(d);
            C_A[i].set(std::__popcount(i), A[i]);
            C_B[i] = kusu_lib::StaticPolynomial<T>(d);
            C_B[i].set(std::__popcount(i), B[i]);
        }
        auto C = or_convolution(C_A, C_B);
        std::vector<T> res(N);
        for(int i = 0; i < N; i++) res[i] = C[i].get(std::__popcount(i));
        return res;
    }

    template<class T> static void Hadamard_transform(std::vector<T>& A) {
        int N = A.size();
        assert(std::__popcount(N) == 1);
        for(int i = 1; i != N; i <<= 1) {
            for(int j = 0; j != N; j += i << 1) {
                for(int k = 0; k != i; k++) {
                    T x = A[j + k];
                    T y = A[i + j + k];
                    A[j + k] = x + y;
                    A[i + j + k] = x - y;
                }
            }
        }
    }

    template<class T, class F>
    static std::vector<T> xor_convolution(const std::vector<T>& A, const std::vector<T>& B,
                                          F divide_by_N) {
        assert(A.size() == B.size());
        int N = A.size();
        assert(std::__popcount(N) == 1);
        int d = 31 - std::__countl_zero(N);
        auto C_A = A;
        auto C_B = B;
        Hadamard_transform(C_A);
        Hadamard_transform(C_B);
        std::vector<T> C(N);
        for(int i = 0; i < N; i++) C[i] = C_A[i] * C_B[i];
        Hadamard_transform(C);
        for(int i = 0; i < N; i++) C[i] = divide_by_N(C[i]);
        return C;
    }

    template<class T>
    static std::vector<T> xor_convolution(const std::vector<T>& A, const std::vector<T>& B) {
        assert(A.size() == B.size());
        int N = A.size();
        assert(std::__popcount(N) == 1);
        std::function<T(T)> divide_by_N = [&](T x) { return x / N; };
        return xor_convolution(A, B, divide_by_N);
    }
};
}//namespace kusu_lib