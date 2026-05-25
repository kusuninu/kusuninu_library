#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<typename T, bool IsMinPlus = true>
struct Tropical {
  private:
    T _val;
    bool _is_inf;
  public:
    static constexpr bool has_numeric_limits = std::numeric_limits<T>::is_specialized;

    Tropical() {
        if constexpr(has_numeric_limits) {
            _val = std::numeric_limits<T>::max() / 2;
            _is_inf = false;
        }
        else {
            _val = T();
            _is_inf = true;
        }
    }

    Tropical(T v) : _val(v), _is_inf(false) {}

    Tropical(const Tropical& other) {
        _val = other._val;
        _is_inf = other._is_inf;
    }

    T val(){return _val;}

    bool is_inf(){
        if constexpr(has_numeric_limits) {
            return _val >= std::numeric_limits<T>::max() / 4;
        }
        else return _is_inf;
    }
    

    Tropical& operator=(const Tropical& other) {
        if(this != &other) {
            _val = other._val;
            _is_inf = other._is_inf;
        }
        return *this;
    }

    Tropical& operator=(const T& other) {
        _val = other;
        return *this;
    }

    Tropical& operator+=(const Tropical& other) {
        if constexpr(IsMinPlus) {
            if constexpr(has_numeric_limits) { _val = std::min(_val, other._val); }
            else {
                if(_is_inf) *this = other;
                else if(!other._is_inf) _val = std::min(_val, other._val);
            }
        }
        else {
            if constexpr(has_numeric_limits) { _val = std::max(_val, other._val); }
            else {
                if(_is_inf) *this = other;
                else if(!other._is_inf) _val = std::max(_val, other._val);
            }
        }
        return *this;
    }

    Tropical& operator*=(const Tropical& other) {
        if constexpr(has_numeric_limits) {
            if(_val >= std::numeric_limits<T>::max() / 4 || other._val >= std::numeric_limits<T>::max() / 4) {
                *this = Tropical();
            }
            else { _val += other._val; }
        }
        else {
            if(_is_inf || other._is_inf) { *this = Tropical(); }
            else { _val += other._val; }
        }
        return *this;
    }

    Tropical operator+(const Tropical& other) const { return Tropical(*this) += other; }

    Tropical operator*(const Tropical& other) const { return Tropical(*this) *= other; }

    bool operator==(const Tropical& other) const {
        return (_is_inf && other._is_inf) || (!_is_inf && !other._is_inf && _val == other._val);
    }
    bool operator!=(const Tropical& other) const { return !(*this == other); }
    bool operator<(const Tropical& other) const {
        if(_is_inf) return !other._is_inf;
        if(other._is_inf) return false;
        return _val < other._val;
    }
    bool operator>(const Tropical& other) const { return other < *this; }
    bool operator<=(const Tropical& other) const { return !(other < *this); }
    bool operator>=(const Tropical& other) const { return !(*this < other); }

    friend std::ostream& operator<<(std::ostream& os, const Tropical& x) {
        if constexpr(has_numeric_limits) {
            if(x._val >= std::numeric_limits<T>::max() / 4) os << "INF";
            else os << x._val;
        }
        else {
            if(x._is_inf) os << "INF";
            else os << x._val;
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

    static Tropical zero() { return Tropical(); }
    static Tropical one() { return Tropical(T(0)); }
};
}//namespace kusu_lib