#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<typename T> class Montgomery {
    static_assert(std::is_unsigned<T>::value, "Unsigned integer required");

  public:
    using WideT = typename std::conditional<sizeof(T) <= 4, uint64_t, __uint128_t>::type;

    T modulus;
    T inv;// -modulus^-1 mod R
    T r2;// R^2 mod modulus

    Montgomery(T mod) : modulus(mod) {
        assert(mod % 2 == 1);
        inv = montgomery_inv(mod);
        WideT R = WideT(1) << (sizeof(T) * 8);
        r2 = R % modulus;
        r2 = WideT(r2) * r2 % modulus;
    }

    struct MontgomeryInt {
        Montgomery* mont;
        T value;
        template<typename U, typename = typename std::enable_if<std::is_integral<U>::value>::type>
        MontgomeryInt(U v, Montgomery* m) : mont(m) {
            if(v >= 0) { value = mont->montgomery_mul(static_cast<T>(v), mont->r2); }
            else {
                auto neg = mont->montgomery_mul(static_cast<T>(-v), mont->r2);
                if(neg == 0) { value = 0; }
                else { value = mont->modulus - neg; }
            }
        }

        explicit MontgomeryInt(Montgomery* m) : mont(m), value(0) {}

        T val() const { return mont->montgomery_reduce(value); }

        T raw() const { return value; }

        friend std::ostream& operator<<(std::ostream& os, const MontgomeryInt& x) {
            return os << x.val();
        }

        friend std::istream& operator>>(std::istream& is, MontgomeryInt& x) {
            long long v;
            is >> v;
            if(!is) return is;
            assert(x.mont);
            x = v;
            return is;
        }

        template<typename U, typename = typename std::enable_if<std::is_integral<U>::value>::type>
        MontgomeryInt& operator=(U v) {
            assert(mont);
            if(v >= 0) { value = mont->montgomery_mul(static_cast<T>(v), mont->r2); }
            else {
                auto neg = mont->montgomery_mul(static_cast<T>(-v), mont->r2);
                if(neg == 0) { value = 0; }
                else { value = mont->modulus - neg; }
            }
            return *this;
        }

        MontgomeryInt& operator+=(const MontgomeryInt& other) {
            check(other);
            value += other.value;
            if(value >= mont->modulus) value -= mont->modulus;
            return *this;
        }

        MontgomeryInt operator+(const MontgomeryInt& other) const {
            MontgomeryInt res = *this;
            res += other;
            return res;
        }

        MontgomeryInt& operator-=(const MontgomeryInt& other) {
            check(other);
            value =
                value >= other.value ? value - other.value : value + mont->modulus - other.value;
            return *this;
        }

        MontgomeryInt operator-(const MontgomeryInt& other) const {
            MontgomeryInt res = *this;
            res -= other;
            return res;
        }

        MontgomeryInt& operator*=(const MontgomeryInt& other) {
            check(other);
            value = mont->montgomery_mul(value, other.value);
            return *this;
        }

        MontgomeryInt operator*(const MontgomeryInt& other) const {
            MontgomeryInt res = *this;
            res *= other;
            return res;
        }

        MontgomeryInt& operator/=(const MontgomeryInt& other) {
            check(other);
            *this *= other.inverse();
            return *this;
        }

        MontgomeryInt operator/(const MontgomeryInt& other) const {
            MontgomeryInt res = *this;
            res /= other;
            return res;
        }

        bool operator==(const MontgomeryInt& other) const {
            check(other);
            return value == other.value;
        }

        bool operator!=(const MontgomeryInt& other) const { return !(*this == other); }

        MontgomeryInt pow(uint64_t exp) const {
            MontgomeryInt base = *this;
            MontgomeryInt result(1, mont);
            while(exp) {
                if(exp & 1) result *= base;
                base *= base;
                exp >>= 1;
            }
            return result;
        }

        MontgomeryInt inverse() const { return pow(mont->modulus - 2); }

      private:
        void check(const MontgomeryInt& other) const { assert(mont == other.mont); }
    };

    T montgomery_mul(T a, T b) const {
        WideT t = WideT(a) * b;// 32bit -> 64bit, 64bit -> 128bit
        return montgomery_reduce(t);
    }

    T montgomery_reduce(WideT t) const {
        T m = T(t) * inv;
        WideT u = t + WideT(m) * modulus;
        T res = T(u >> (sizeof(T) * 8));
        if(res >= modulus) res -= modulus;
        return res;
    }

  private:
    T montgomery_inv(T m) const {
        T x = 1;
        for(int i = 0; i < (sizeof(T) <= 4 ? 5 : 6); i++) x *= 2 - m * x;
        return -x;
    }
};

template<typename T> typename Montgomery<T>::MontgomeryInt make_mont(T value, Montgomery<T>* mont) {
    return typename Montgomery<T>::MontgomeryInt(value, mont);
}
}//namespace kusu_lib