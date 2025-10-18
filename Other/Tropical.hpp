#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<typename T, bool IsMinPlus = true> struct Tropical {
    T val;
    bool is_inf;

    static constexpr bool has_numeric_limits = std::numeric_limits<T>::is_specialized;

    Tropical() {
        if constexpr(has_numeric_limits) {
            val = std::numeric_limits<T>::max() / 2;
            is_inf = false;
        }
        else {
            val = T();
            is_inf = true;
        }
    }

    Tropical(T v) : val(v), is_inf(false) {}

    Tropical(const Tropical& other) {
        val = other.val;
        is_inf = other.is_inf;
    }

    Tropical& operator=(const Tropical& other) {
        if(this != &other) {
            val = other.val;
            is_inf = other.is_inf;
        }
        return *this;
    }

    Tropical& operator=(const T& other) {
        val = other;
        return *this;
    }

    Tropical& operator+=(const Tropical& other) {
        if constexpr(IsMinPlus) {
            if constexpr(has_numeric_limits) { val = std::min(val, other.val); }
            else {
                if(is_inf) *this = other;
                else if(!other.is_inf) val = std::min(val, other.val);
            }
        }
        else {
            if constexpr(has_numeric_limits) { val = std::max(val, other.val); }
            else {
                if(is_inf) *this = other;
                else if(!other.is_inf) val = std::max(val, other.val);
            }
        }
        return *this;
    }

    Tropical& operator*=(const Tropical& other) {
        if constexpr(has_numeric_limits) {
            if(val >= std::numeric_limits<T>::max() / 4 ||
               other.val >= std::numeric_limits<T>::max() / 4) {
                *this = Tropical();
            }
            else { val += other.val; }
        }
        else {
            if(is_inf || other.is_inf) {
                *this = Tropical();
            }
            else { val += other.val; }
        }
        return *this;
    }

    Tropical operator+(const Tropical& other) const { return Tropical(*this) += other; }

    Tropical operator*(const Tropical& other) const { return Tropical(*this) *= other; }

    bool operator==(const Tropical& other) const {
        return (is_inf && other.is_inf) || (!is_inf && !other.is_inf && val == other.val);
    }
    bool operator!=(const Tropical& other) const { return !(*this == other); }
    bool operator<(const Tropical& other) const {
        if(is_inf) return !other.is_inf;
        if(other.is_inf) return false;
        return val < other.val;
    }
    bool operator>(const Tropical& other) const { return other < *this; }
    bool operator<=(const Tropical& other) const { return !(other < *this); }
    bool operator>=(const Tropical& other) const { return !(*this < other); }

    friend std::ostream& operator<<(std::ostream& os, const Tropical& x) {
        if constexpr(has_numeric_limits) {
            if(x.val >= std::numeric_limits<T>::max() / 4) os << "INF";
            else os << x.val;
        }
        else {
            if(x.is_inf) os << "INF";
            else os << x.val;
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Tropical& x) {
        std::string s;
        is >> s;
        if(s == "INF") { x = Tropical(); }
        else {
            T v;
            if constexpr(std::is_same_v<T, int>) v = std::stoi(s);
            else if constexpr(std::is_same_v<T, long long>) v = std::stoll(s);
            else if constexpr(std::is_same_v<T, double>) v = std::stod(s);
            else is >> v;
            x = Tropical(v);
        }
        return is;
    }
};
}//namespace kusu_lib