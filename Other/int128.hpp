#pragma once
#include <cctype>
#include <iostream>
#include <limits>
#include <string>

inline std::ostream& operator<<(std::ostream& os, __uint128_t value) {
    if(value == 0) return os << "0";

    char buffer[40];
    int pos = 39;
    buffer[pos] = '\0';
    --pos;

    while(value > 0) {
        buffer[pos--] = '0' + static_cast<int>(value % 10);
        value /= 10;
    }

    return os << (buffer + pos + 1);
}

inline std::ostream& operator<<(std::ostream& os, __int128_t value) {
    if(value < 0) {
        os << '-';
        return os << static_cast<__uint128_t>(-value);
    }
    return os << static_cast<__uint128_t>(value);
}

inline std::istream& operator>>(std::istream& is, __uint128_t& value) {
    value = 0;
    std::string s;
    is >> s;
    if(s.empty()) {
        is.setstate(std::ios::failbit);
        return is;
    }

    for(char c: s) {
        if(!std::isdigit(c)) {
            is.setstate(std::ios::failbit);
            return is;
        }
        __uint128_t prev = value;
        value = value * 10 + (c - '0');
        if(value < prev) {
            is.setstate(std::ios::failbit);
            return is;
        }
    }
    return is;
}

inline std::istream& operator>>(std::istream& is, __int128_t& value) {
    value = 0;
    std::string s;
    is >> s;
    if(s.empty()) {
        is.setstate(std::ios::failbit);
        return is;
    }

    size_t pos = 0;
    bool negative = false;
    if(s[0] == '-') {
        negative = true;
        ++pos;
        if(pos == s.size()) {
            is.setstate(std::ios::failbit);
            return is;
        }
    }

    __uint128_t tmp = 0;
    for(; pos < s.size(); ++pos) {
        char c = s[pos];
        if(!std::isdigit(c)) {
            is.setstate(std::ios::failbit);
            return is;
        }
        __uint128_t prev = tmp;
        tmp = tmp * 10 + (c - '0');
        if(tmp < prev) {
            is.setstate(std::ios::failbit);
            return is;
        }
    }

    if(negative) {
        if(tmp > static_cast<__uint128_t>(std::numeric_limits<__int128_t>::max()) + 1) {
            is.setstate(std::ios::failbit);
            return is;
        }
        value = -static_cast<__int128_t>(tmp);
    }
    else {
        if(tmp > static_cast<__uint128_t>(std::numeric_limits<__int128_t>::max())) {
            is.setstate(std::ios::failbit);
            return is;
        }
        value = static_cast<__int128_t>(tmp);
    }

    return is;
}

/*
namespace std {

template<>
struct is_integral<__int128_t> : std::true_type {};

template<>
struct is_integral<__uint128_t> : std::true_type {};

}//namespace std

namespace std {

template<>
class numeric_limits<__int128_t> {
  public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = true;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr bool is_modulo = false;

    static constexpr int digits = 127;
    static constexpr int digits10 = 38;
    static constexpr int radix = 2;

    static constexpr __int128_t min() noexcept { return ((__int128_t)1 << 127) * -1; }
    static constexpr __int128_t lowest() noexcept { return min(); }
    static constexpr __int128_t max() noexcept { return (((__int128_t)1 << 127) - 1); }

    static constexpr __int128_t epsilon() noexcept { return 0; }
    static constexpr __int128_t round_error() noexcept { return 0; }
    static constexpr __int128_t infinity() noexcept { return 0; }
    static constexpr __int128_t quiet_NaN() noexcept { return 0; }
    static constexpr __int128_t signaling_NaN() noexcept { return 0; }
    static constexpr __int128_t denorm_min() noexcept { return 0; }
};

template<>
class numeric_limits<__uint128_t> {
  public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr bool is_modulo = true;

    static constexpr int digits = 128;
    static constexpr int digits10 = 38;
    static constexpr int radix = 2;

    static constexpr __uint128_t min() noexcept { return 0; }
    static constexpr __uint128_t lowest() noexcept { return 0; }
    static constexpr __uint128_t max() noexcept { return (((__uint128_t)1 << 127) * 2 - 1); }

    static constexpr __uint128_t epsilon() noexcept { return 0; }
    static constexpr __uint128_t round_error() noexcept { return 0; }
    static constexpr __uint128_t infinity() noexcept { return 0; }
    static constexpr __uint128_t quiet_NaN() noexcept { return 0; }
    static constexpr __uint128_t signaling_NaN() noexcept { return 0; }
    static constexpr __uint128_t denorm_min() noexcept { return 0; }
};

}//namespace std
*/