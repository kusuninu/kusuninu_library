#include <bits/stdc++.h>
template<class T> struct Matrix {
  private:
    int N;
    int M;
    T* A;
    mutable bool pow_initialized = false;
    mutable T* temp1;
    mutable T* temp2;
    mutable T* temp3;
    mutable T* temp4;

    //X is n*m matrix and Y is m*l matrix;result of X*Y that is n*l is in Z
    //memory alloc must have been done
    static void fast_mul(int n, int m, int l, const T X[], const T Y[], T Z[]) {
        for(int i = 0; i < n * l; i++) Z[i] = 0;
        for(int i = 0; i < n; i++) {
            for(int k = 0; k < m; k++) {
                for(int j = 0; j < l; j++) { Z[i * l + j] += X[i * m + k] * Y[k * l + j]; }
            }
        }
    }

    struct Row {
        T* row;
        int p;
        Row(T* row, int p) : row(row), p(p) {}
        Row(const T* row, int p) : row(row), p(p) {}
        T& operator[](int j) {
            assert(0 <= j && j < p);
            return row[j];
        }
        const T& operator[](int j) const {
            assert(0 <= j && j < p);
            return row[j];
        }
    };

  public:
    Matrix(int _N, int _M) : Matrix(_N, _M, (T)0) {}

    Matrix(int _N, int _M, T c) {
        assert(0 <= _N);
        assert(0 <= _M);
        N = _N;
        M = _M;
        A = new T[N * M];
        for(int i = 0; i < N * M; i++) A[i] = c;
    }

    Matrix(const std::vector<std::vector<T>>& V) {
        int p = 0;
        int pre = -1;
        for(const auto& v: V) {
            assert(pre == -1 || pre == v.size());
            p += v.size();
            pre = v.size();
        }
        A = new T[p];
        for(int idx = 0; const auto& v: V)
            for(auto x: v) {
                A[idx] = x;
                idx++;
            }
        N = V.size();
        if(pre == -1) M = 0;
        else
            M = pre;
    }

    template<class Ptr, std::enable_if_t<std::is_same_v<Ptr, T*>, int> = 0>
    Matrix(int _N, int _M, const T* V) {
        assert(0 <= _N);
        assert(0 <= _M);
        N = _N;
        M = _M;
        A = new T[N * M];
        for(int i = 0; i < N * M; i++) A[i] = V[i];
    }

    Matrix(int _N, int _M, const std::vector<T>& V) {
        assert(0 <= _N);
        assert(0 <= _M);
        assert(V.size() == _N * _M);
        N = _N;
        M = _M;
        A = new T[N * M];
        for(int i = 0; i < N * M; i++) A[i] = V[i];
    }

    int height() const { return N; }

    int width() const { return M; }

    Row operator[](int i) {
        assert(0 <= i && i < N);
        return Row(&A[i * M], M);
    }

    const Row operator[](int i) const {
        assert(0 <= i && i < N);
        return Row(&A[i * M], M);
    }

    void set(int i, int j, T t) {
        assert(0 <= i && i < N);
        assert(0 <= j && j < M);
        A[i * M + j] = t;
    }

    T get(int i, int j) const {
        assert(0 <= i && i < N);
        assert(0 <= j && j < M);
        return A[i * M + j];
    }

    static Matrix Identity_Matrix(int n) {
        Matrix<T> B(n, n, 0);
        for(int i = 0; i < n; i++) B.set(i, i, 1);
        return B;
    }

    static Matrix Zero_Matrix(int n) { return Matrix<T>(n, n, 0); }

    void clear_row(int i) {
        assert(0 <= i && i < N);
        for(int j = 0; j < M; j++) A[i * M + j] = 0;
    }

    void clear_column(int i) {
        assert(0 <= i && i < M);
        for(int j = 0; j < N; j++) A[j * M + i] = 0;
    }

    template<class U> Matrix<T>& operator+=(const Matrix<U>& B) {
        assert(N == B.N && M == B.M);
        for(int i = 0; i < N * M; i++) A[i] += B.A[i];
        return *this;
    }

    template<class U> Matrix<T>& operator-=(const Matrix<U>& B) {
        assert(N == B.N && M == B.M);
        for(int i = 0; i < N * M; i++) A[i] -= B.A[i];
        return *this;
    }

    template<class U> Matrix<T>& operator*=(const U& v) {
        for(int i = 0; i < N * M; i++) A[i] *= v;
        return *this;
    }

    Matrix operator*(const Matrix<T>& B) const {
        assert(M == B.height());
        int W = B.width();
        T C[N * W];
        fast_mul(N, M, W, A, B.A, C);
        return Matrix(N, W, C);
    }

    Matrix pow(long long n) const {
        assert(0 <= n);
        assert(N == M);
        if(!pow_initialized) {
            pow_initialized = true;
            temp1 = new T[N * N];
            temp2 = new T[N * N];
            temp3 = new T[N * N];
            temp4 = new T[N * N];
        }
        bool k = 0;
        bool l = 0;
        memcpy(temp3, A, sizeof(T) * N * N);
        for(int i = 0; i < N * N; i++) temp1[i] = i % (N + 1) == 0;
        while(n) {
            if(n & 1) {
                if(k) {
                    if(l) fast_mul(N, N, N, temp2, temp4, temp1);
                    else
                        fast_mul(N, N, N, temp1, temp4, temp2);
                } else {
                    if(l) fast_mul(N, N, N, temp2, temp3, temp1);
                    else
                        fast_mul(N, N, N, temp1, temp3, temp2);
                }
                l ^= 1;
            }

            if(k) fast_mul(N, N, N, temp4, temp4, temp3);
            else
                fast_mul(N, N, N, temp3, temp3, temp4);
            n >>= 1;
            k ^= 1;
        }
        if(l) {
            return Matrix(N, N, temp2);
        } else {
            return Matrix(N, N, temp1);
        }
    }

    Matrix<T> Transpose() {
        Matrix<T> B(M, N, 0);
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < M; j++) { B.set(j, i, A[i * M + j]); }
        }
        return B;
    }
};
template<class T, class U>
Matrix<typename std::common_type<T, U>::type> operator+(const Matrix<T>& lhs,
                                                        const Matrix<U>& rhs) {
    return Matrix<typename std::common_type<T, U>::type>(lhs) += rhs;
}
template<class T, class U>
Matrix<typename std::common_type<T, U>::type> operator-(const Matrix<T>& lhs,
                                                        const Matrix<U>& rhs) {
    return Matrix<typename std::common_type<T, U>::type>(lhs) -= rhs;
}
template<class T, class U>
Matrix<typename std::common_type<T, U>::type> operator*(const Matrix<T>& lhs, const U& rhs) {
    return Matrix<typename std::common_type<T, U>::type>(lhs) *= rhs;
}
template<class T, class U>
Matrix<typename std::common_type<T, U>::type> operator*(const T& lhs, const Matrix<U>& rhs) {
    return Matrix<typename std::common_type<T, U>::type>(rhs) *= lhs;
}
template<class U> std::ostream& operator<<(std::ostream& os, const Matrix<U>& A) {
    for(int i = 0; i < A.height() * A.width(); i++)
        os << A.get(i / A.width(), i % A.width()) << ((i + 1) % A.width() ? " " : "\n");
    return os;
}