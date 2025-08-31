#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<class T = long long> struct fraction;

template<class T> struct fraction {
  private:
    T numerator;
    T denominator;

    template<class U> friend struct fraction;

    template<class U, class V>
    friend bool operator<(const fraction<U>& lhs, const fraction<V>& rhs);
    template<class U, class V>
    friend bool operator==(const fraction<U>& lhs, const fraction<V>& rhs);
    template<class U> friend std::ostream& operator<<(std::ostream& os, const fraction<U>& f);

  public:
    fraction(T i = 0) : numerator(i), denominator(1) {}

    fraction(T numerator, T denominator) : numerator(numerator), denominator(denominator) {
        assert(denominator != 0);
        if(denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }
    template<class U>
    fraction(fraction<U> f) : numerator(f.numerator), denominator(f.denominator) {}

    fraction<T> reciprocal() { return fraction<T>(denominator, numerator); }

    std::pair<T, T> get() const { return std::make_pair(numerator, denominator); }

    T get_numerator() const { return numerator; }

    T get_denominator() const { return denominator; }

    fraction<T> operator+() const { return *this; }

    fraction<T> operator-() const { return fraction<T>(-numerator, denominator); }

    template<class U> fraction<T>& operator+=(const fraction<U>& e) {
        numerator = numerator * e.denominator + denominator * e.numerator,
        denominator *= e.denominator;
        return *this;
    }

    template<class U> fraction<T>& operator+=(const U& e) { return *this += fraction<U>(e); }

    template<class U> fraction<T>& operator-=(const fraction<U>& e) {
        numerator = numerator * e.denominator - denominator * e.numerator,
        denominator *= e.denominator;
        return *this;
    }

    template<class U> fraction<T>& operator-=(const U& e) { return *this -= fraction<U>(e); }

    template<class U> fraction<T>& operator*=(const fraction<U>& e) {
        numerator *= e.numerator;
        denominator *= e.denominator;
        return *this;
    }

    template<class U> fraction<T>& operator*=(const U& e) { return *this *= fraction<U>(e); }

    template<class U> fraction<T>& operator/=(const fraction<U>& e) {
        numerator *= e.denominator;
        denominator *= e.numerator;
        return *this;
    }

    template<class U> fraction<T>& operator/=(const U& e) { return *this /= fraction<U>(e); }

    operator long long() const { return (long long)numerator / (long long)denominator; }

    operator long double() const { return (long double)numerator / (long double)denominator; }
};

template<class U, class V> bool operator<(const fraction<U>& lhs, const fraction<V>& rhs) {
    return lhs.numerator * rhs.denominator < lhs.denominator * rhs.numerator;
}
template<class U, class V> bool operator<(const fraction<U>& lhs, const V& rhs) {
    return lhs < fraction<typename std::common_type<U, V>::type>(rhs);
}
template<class U, class V> bool operator<(const U& lhs, const fraction<V>& rhs) {
    return fraction<typename std::common_type<U, V>::type>(lhs) < rhs;
}
template<class U, class V> bool operator<=(const fraction<U>& lhs, const fraction<V>& rhs) {
    return !(rhs < lhs);
}
template<class U, class V> bool operator<=(const fraction<U>& lhs, const V& rhs) {
    return !(rhs < lhs);
}
template<class U, class V> bool operator<=(const U& lhs, const fraction<V>& rhs) {
    return !(rhs < lhs);
}
template<class U, class V> bool operator>(const fraction<U>& lhs, const fraction<V>& rhs) {
    return rhs < lhs;
}
template<class U, class V> bool operator>(const fraction<U>& lhs, const V& rhs) {
    return rhs < lhs;
}
template<class U, class V> bool operator>(const U& lhs, const fraction<V>& rhs) {
    return rhs < lhs;
}
template<class U, class V> bool operator>=(const fraction<U>& lhs, const fraction<V>& rhs) {
    return !(lhs < rhs);
}
template<class U, class V> bool operator>=(const fraction<U>& lhs, const V& rhs) {
    return !(lhs < rhs);
}
template<class U, class V> bool operator>=(const U& lhs, const fraction<V>& rhs) {
    return !(lhs < rhs);
}
template<class U, class V> bool operator==(const fraction<U>& lhs, const fraction<V>& rhs) {
    return lhs.numerator * rhs.denominator == lhs.denominator * rhs.numerator;
}
template<class U, class V> bool operator==(const fraction<U>& lhs, const V& rhs) {
    return lhs == fraction<typename std::common_type<U, V>::type>(rhs);
}
template<class U, class V> bool operator==(const U& lhs, const fraction<V>& rhs) {
    return fraction<typename std::common_type<U, V>::type>(lhs) == rhs;
}
template<class U, class V> bool operator!=(const fraction<U>& lhs, const fraction<V>& rhs) {
    return !(lhs == rhs);
}
template<class U, class V> bool operator!=(const fraction<U>& lhs, const V& rhs) {
    return !(lhs == rhs);
}
template<class U, class V> bool operator!=(const U& lhs, const fraction<V>& rhs) {
    return !(lhs == rhs);
}
template<class U, class V>
fraction<typename std::common_type<U, V>::type> operator+(const fraction<U>& lhs,
                                                          const fraction<V>& rhs) {
    return fraction<typename std::common_type<U, V>::type>(lhs) += rhs;
}
template<class U, class V>
fraction<typename std::common_type<U, V>::type> operator+(const fraction<U>& lhs, const V& rhs) {
    return fraction<typename std::common_type<U, V>::type>(lhs) += rhs;
}
template<class U, class V>
fraction<typename std::common_type<U, V>::type> operator+(const U& lhs, const fraction<V>& rhs) {
    return fraction<typename std::common_type<U, V>::type>(lhs) += rhs;
}
template<class U, class V>
fraction<typename std::common_type<U, V>::type> operator-(const fraction<U>& lhs,
                                                          const fraction<V>& rhs) {
    return fraction<typename std::common_type<U, V>::type>(lhs) -= rhs;
}
template<class U, class V>
fraction<typename std::common_type<U, V>::type> operator-(const fraction<U>& lhs, const V& rhs) {
    return fraction<typename std::common_type<U, V>::type>(lhs) -= rhs;
}
template<class U, class V>
fraction<typename std::common_type<U, V>::type> operator-(const U& lhs, const fraction<V>& rhs) {
    return fraction<typename std::common_type<U, V>::type>(lhs) -= rhs;
}
template<class U, class V>
fraction<typename std::common_type<U, V>::type> operator*(const fraction<U>& lhs,
                                                          const fraction<V>& rhs) {
    return fraction<typename std::common_type<U, V>::type>(lhs) *= rhs;
}
template<class U, class V>
fraction<typename std::common_type<U, V>::type> operator*(const fraction<U>& lhs, const V& rhs) {
    return fraction<typename std::common_type<U, V>::type>(lhs) *= rhs;
}
template<class U, class V>
fraction<typename std::common_type<U, V>::type> operator*(const U& lhs, const fraction<V>& rhs) {
    return fraction<typename std::common_type<U, V>::type>(lhs) *= rhs;
}
template<class U, class V>
fraction<typename std::common_type<U, V>::type> operator/(const fraction<U>& lhs,
                                                          const fraction<V>& rhs) {
    return fraction<typename std::common_type<U, V>::type>(lhs) /= rhs;
}
template<class U, class V>
fraction<typename std::common_type<U, V>::type> operator/(const fraction<U>& lhs, const V& rhs) {
    return fraction<typename std::common_type<U, V>::type>(lhs) /= rhs;
}
template<class U, class V>
fraction<typename std::common_type<U, V>::type> operator/(const U& lhs, const fraction<V>& rhs) {
    return fraction<typename std::common_type<U, V>::type>(lhs) /= rhs;
}
template<class U> std::ostream& operator<<(std::ostream& os, const fraction<U>& f) {
    return os << f.numerator << "/" << f.denominator;
}
}//namespace kusu_lib