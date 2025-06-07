#include <bits/stdc++.h>
struct LinearSieve {
  private:
    int* min_factor;
    int _N;
    std::vector<int> primes;

  public:
    LinearSieve() : _N(-1) {}

    LinearSieve(int N) : _N(N) {
        assert(1 <= _N);
        min_factor = new int[_N + 1];
        for(int i = 0; i <= _N; i++) min_factor[i] = -1;
        for(int i = 2; i <= _N; i++) {
            if(min_factor[i] == -1) {
                min_factor[i] = i;
                primes.push_back(i);
            }
            for(int p: primes) {
                if(p * i > N || p > min_factor[i]) break;
                min_factor[p * i] = p;
            }
        }
    }

    bool is_prime(int x) {
        assert(0 <= x && x <= _N);
        return min_factor[x] == x;
    }

    std::vector<int> get_primes() { return primes; }

    std::vector<int> get_totient() {
        assert(1 <= _N);
        std::vector<int> totient(_N + 1);
        totient[1] = 1;
        for(int i = 2; i <= _N; i++) {
            int k = min_factor[i];
            if(k == i) totient[i] = i - 1;
            else if(min_factor[i / k] == k)
                totient[i] = totient[i / k] * k;
            else
                totient[i] = totient[i / k] * (k - 1);
        }
        return totient;
    }

    std::vector<std::pair<int, int>> prime_factorize(int x) {
        assert(1 <= x && x <= _N);
        std::vector<std::pair<int, int>> res;
        int c = 0;
        int p = 0;
        while(1 < x) {
            c = 0;
            p = min_factor[x];
            while(x % p == 0) {
                x /= p;
                c++;
            }
            res.emplace_back(p, c);
        }
        return res;
    }

    std::vector<int> get_divisors(int x) {
        assert(1 <= x && x <= _N);
        auto factors = prime_factorize(x);
        std::vector<int> res = {1};
        for(auto pf: factors) {
            for(int a: res) {
                for(int i = 0; i < pf.second; i++) {
                    a *= pf.first;
                    res.push_back(a);
                }
            }
        }
        return res;
    }

    int count_divisors(int x) {
        assert(1 <= x && x <= _N);
        int res = 1;
        int c = 0;
        int p = 0;
        while(1 < x) {
            c = 0;
            p = min_factor[x];
            while(x % p == 0) {
                x /= p;
                c++;
            }
            res *= c + 1;
        }
        return res;
    }

    template<class T> void sum_over_divisors(std::vector<T>& A) {
        int N = (int)A.size() - 1;
        assert(N <= _N);
        for(int i = 1; i <= N; i++) {
            if(!is_prime(i)) continue;
            for(int j = 1; j * i <= N; j++) A[j * i] += A[j];
        }
    }

    template<class T, class F> void sum_over_divisors(std::vector<T>& A, F f) {
        int N = (int)A.size() - 1;
        assert(N <= _N);
        for(int i = 1; i <= N; i++) {
            if(!is_prime(i)) continue;
            for(int j = 1; j * i <= N; j++) A[j * i] = f(A[j * i], A[j]);
        }
    }

    template<class T> void inverse_sum_over_divisors(std::vector<T>& A) {
        int N = (int)A.size() - 1;
        assert(N <= _N);
        for(int i = 1; i <= N; i++) {
            if(!is_prime(i)) continue;
            for(int j = N / i; j >= 1; j--) A[j * i] -= A[j];
        }
    }

    //f(a,b) = c <=> f_inv(c,b) = a
    template<class T, class F> void inverse_sum_over_divisors(std::vector<T>& A, F f_inv) {
        int N = (int)A.size() - 1;
        assert(N <= _N);
        for(int i = 1; i <= N; i++) {
            if(!is_prime(i)) continue;
            for(int j = N / i; j >= 1; j--) A[j * i] = f_inv(A[j * i], A[j]);
        }
    }

    template<class T> void sum_over_multiples(std::vector<T>& A) {
        int N = (int)A.size() - 1;
        assert(N <= _N);
        for(int i = 1; i <= N; i++) {
            if(!is_prime(i)) continue;
            for(int j = N / i; j >= 1; j--) A[j] += A[j * i];
        }
    }

    template<class T, class F> void sum_over_multiples(std::vector<T>& A, F f) {
        int N = (int)A.size() - 1;
        assert(N <= _N);
        for(int i = 1; i <= N; i++) {
            if(!is_prime(i)) continue;
            for(int j = N / i; j >= 1; j--) A[j] = f(A[j], A[j * i]);
        }
    }

    template<class T> void inverse_sum_over_multiples(std::vector<T>& A) {
        int N = (int)A.size() - 1;
        assert(N <= _N);
        for(int i = 1; i <= N; i++) {
            if(!is_prime(i)) continue;
            for(int j = 1; j * i <= N; j++) A[j] -= A[j * i];
        }
    }

    //f(a,b) = c <=> f_inv(c,b) = a
    template<class T, class F> void inverse_sum_over_multiples(std::vector<T>& A, F f_inv) {
        int N = (int)A.size() - 1;
        assert(N <= _N);
        for(int i = 1; i <= N; i++) {
            if(!is_prime(i)) continue;
            for(int j = 1; j * i <= N; j++) A[j] = f_inv(A[j], A[j * i]);
        }
    }

    template<class T>
    std::vector<T> gcd_convolution(const std::vector<T>& A, const std::vector<T>& B) {
        int N = max(A.size(), B.size());
        assert(N - 1 <= _N);
        std::vector<T> C_A(N, 0), C_B(N, 0), C(N, 0);
        for(int i = 1; i < A.size(); i++) C_A[i] = A[i];
        for(int i = 1; i < B.size(); i++) C_B[i] = B[i];
        sum_over_multiples(C_A);
        sum_over_multiples(C_B);
        for(int i = 1; i < N; i++) C[i] = C_A[i] * C_B[i];
        inverse_sum_over_multiples(C);
        return C;
    }

    //sum(a,b) = c <=> sum_inv(c,b) = a
    //T,sum,mul must be a ring
    template<class T, T (*sum)(T, T), T (*sum_inv)(T, T), T (*mul)(T, T), T (*e)()>
    std::vector<T> gcd_convolution(const std::vector<T>& A, const std::vector<T>& B) {
        int N = max(A.size(), B.size());
        assert(N - 1 <= _N);
        std::vector<T> C_A(N, e()), C_B(N, e()), C(N, e());
        for(int i = 1; i < A.size(); i++) C_A[i] = A[i];
        for(int i = 1; i < B.size(); i++) C_B[i] = B[i];
        sum_over_multiples(C_A, sum);
        sum_over_multiples(C_B, sum);
        for(int i = 1; i < N; i++) C[i] = mul(C_A[i], C_B[i]);
        inverse_sum_over_multiples(C, sum_inv);
        return C;
    }

    template<class T>
    std::vector<T> lcm_convolution(const std::vector<T>& A, const std::vector<T>& B) {
        int N = max(A.size(), B.size());
        assert(N - 1 <= _N);
        std::vector<T> C_A(N, 0), C_B(N, 0), C(N, 0);
        for(int i = 1; i < A.size(); i++) C_A[i] = A[i];
        for(int i = 1; i < B.size(); i++) C_B[i] = B[i];
        sum_over_divisors(C_A);
        sum_over_divisors(C_B);
        for(int i = 1; i < N; i++) C[i] = C_A[i] * C_B[i];
        inverse_sum_over_divisors(C);
        return C;
    }

    //sum(a,b) = c <=> sum_inv(c,b) = a
    //T,sum,mul must be a ring
    template<class T, T (*sum)(T, T), T (*sum_inv)(T, T), T (*mul)(T, T), T (*e)()>
    std::vector<T> lcm_convolution(const std::vector<T>& A, const std::vector<T>& B) {
        int N = max(A.size(), B.size());
        assert(N - 1 <= _N);
        std::vector<T> C_A(N, e()), C_B(N, e()), C(N, e());
        for(int i = 1; i < A.size(); i++) C_A[i] = A[i];
        for(int i = 1; i < B.size(); i++) C_B[i] = B[i];
        sum_over_divisors(C_A, sum);
        sum_over_divisors(C_B, sum);
        for(int i = 1; i < N; i++) C[i] = mul(C_A[i], C_B[i]);
        inverse_sum_over_divisors(C, sum_inv);
        return C;
    }
};