#pragma once
#include <bits/stdc++.h>
#include "int128.hpp"

namespace kusu_lib {

template<int Bits>
class int_t;

template<typename T>
struct is_int_t : std::false_type {};

template<int M>
struct is_int_t<int_t<M>> : std::true_type {};

template<class T>
inline constexpr bool is_int_t_v = is_int_t<T>::value;

#pragma pack(push, 1)
template<int Bits>
struct uint_type_picker {
    static_assert(0 < Bits && Bits <= 128);

    static constexpr int LowBits = (64 <= Bits ? 64 : (32 <= Bits ? 32 : (16 <= Bits ? 16 : 8)));
    static constexpr int HighBits = Bits - LowBits;

    using LowType = std::conditional_t<
        LowBits <= 8, uint8_t,
        std::conditional_t<LowBits <= 16, uint16_t,
                           std::conditional_t<LowBits <= 32, uint32_t, uint64_t>>>;

    using HighType = std::conditional_t<
        HighBits == 0, void,
        std::conditional_t<
            HighBits <= 8, uint8_t,
            std::conditional_t<HighBits <= 16, uint16_t,
                               std::conditional_t<HighBits <= 32, uint32_t, uint64_t>>>>;

    using CalcType = std::conditional_t<
        Bits <= 8, uint8_t,
        std::conditional_t<
            Bits <= 16, uint16_t,
            std::conditional_t<Bits <= 32, uint32_t,
                               std::conditional_t<Bits <= 64, uint64_t, __uint128_t>>>>;
};

template<int Bits, class Derived = void>
class uint_t;

template<typename T>
struct is_uint_t : std::false_type {};

template<int Bits, class Derived>
struct is_uint_t<uint_t<Bits,Derived>> : std::true_type {};

template<class T>
inline constexpr bool is_uint_t_v = is_uint_t<T>::value;

template<int Bits, class Derived = void>
class uint_t {
  protected:
    // do not set Bits 0
    static_assert(0 <= Bits && Bits <= 128);

    using picker = uint_type_picker<Bits>;
    using LowType = typename picker::LowType;
    using HighType = typename picker::HighType;
    using CalcType = typename picker::CalcType;

    using DerivedType = std::conditional_t<std::is_void_v<Derived>, uint_t<Bits>, Derived>;

    LowType low{};
    std::conditional_t<std::is_void_v<HighType>, char, HighType> high{};

  public:
    constexpr uint_t() noexcept {}

    template<class T>
    constexpr uint_t(T val) noexcept : uint_t(static_cast<CalcType>(val)) {}

    template<std::integral T>
    requires(!is_uint_t_v<T>)
    constexpr uint_t(T val) noexcept {
        using U = std::make_unsigned_t<CalcType>;
        U uval = static_cast<U>(val);
        low = static_cast<LowType>(uval);
        constexpr int bits_of_uval = sizeof(U) * 8;
        if constexpr(!std::is_void_v<HighType>)
            high = (picker::LowBits < bits_of_uval) ? static_cast<HighType>(uval >> picker::LowBits)
                                                    : 0;
    }

    constexpr CalcType to_calc() const noexcept {
        if constexpr(!std::is_void_v<HighType>)
            return (static_cast<CalcType>(high) << picker::LowBits) | low;
        else return static_cast<CalcType>(low);
    }

    template<typename T>
    constexpr operator T() const noexcept
    requires(std::is_integral_v<T> && !is_uint_t_v<T>)
    {
        return static_cast<T>(to_calc());
    }

    constexpr DerivedType operator+() const { return *this; }

    constexpr DerivedType operator-() const {
        uint_t res;

        if constexpr(!std::is_void_v<HighType>) {
            res.high = ~high;
            res.low = ~low;
            ++res;
            return res;
        }
        res.low = ~low;
        ++res;
        return res;
    }

    constexpr DerivedType& operator++() noexcept {
        if constexpr(std::is_void_v<HighType>) { ++low; }
        else {
            if(++low == 0) ++high;
        }
        return *this;
    }

    constexpr DerivedType operator++(int) noexcept {
        uint_t tmp = *this;
        ++(*this);
        return tmp;
    }

    constexpr DerivedType& operator--() noexcept {
        if constexpr(std::is_void_v<HighType>) { --low; }
        else {
            if(low-- == 0) --high;
        }
        return *this;
    }

    constexpr DerivedType operator--(int) noexcept {
        uint_t tmp = *this;
        --(*this);
        return tmp;
    }

    template<typename T>
    constexpr DerivedType& operator+=(const T& rhs) noexcept {
        if(rhs < 0) return *this -= -rhs;
        if constexpr(std::is_same_v<std::decay_t<T>, uint_t<Bits>>) {
            if constexpr(std::is_void_v<HighType>) { low += rhs.low; }
            else {
                LowType old_low = low;
                low += rhs.low;
                high += rhs.high;
                if(low < old_low) ++high;
            }
        }
        else if constexpr(is_uint_t_v<T>) {
            auto val = static_cast<CalcType>(rhs);
            *this += val;
        }
        else if constexpr(std::is_integral_v<T>) {
            using U = std::make_unsigned_t<T>;
            U uval = static_cast<U>(rhs);

            if constexpr(std::is_void_v<HighType>) { low += static_cast<LowType>(uval); }
            else {
                constexpr int bits_of_uval = sizeof(U) * 8;
                LowType add_low = static_cast<LowType>(uval);
                HighType add_high = (picker::LowBits < bits_of_uval)
                                        ? static_cast<HighType>(uval >> picker::LowBits)
                                        : 0;

                LowType old_low = low;
                low += add_low;
                if(low < old_low) ++high;
                high += add_high;
            }
        }
        else { static_assert(true, "Unsupported type"); }
        return *this;
    }

    template<class T>
    constexpr DerivedType& operator-=(const T& rhs) noexcept {
        if(rhs < 0) *this += -rhs;
        if constexpr(std::is_same_v<std::decay_t<T>, uint_t<Bits>>) {
            if constexpr(std::is_void_v<HighType>) { low -= rhs.low; }
            else {
                LowType old_low = low;
                low -= rhs.low;
                if(low > old_low) --high;
                high -= rhs.high;
            }
        }
        else if constexpr(is_uint_t_v<T>) { *this -= static_cast<CalcType>(rhs); }
        else if constexpr(std::is_integral_v<T>) {
            using U = std::make_unsigned_t<T>;
            U uval = static_cast<U>(rhs);
            if constexpr(std::is_void_v<HighType>) { low -= static_cast<LowType>(uval); }
            else {
                constexpr int bits_of_uval = sizeof(U) * 8;
                LowType sub_low = static_cast<LowType>(uval);
                HighType sub_high = (picker::LowBits < bits_of_uval)
                                        ? static_cast<HighType>(uval >> picker::LowBits)
                                        : 0;

                LowType old_low = low;
                low -= sub_low;
                if(low > old_low) --high;
                high -= sub_high;
            }
        }
        else { static_assert(true, "Unsupported type"); }
        return *this;
    }

    template<class T>
    constexpr DerivedType& operator*=(const T& rhs) noexcept {
        CalcType res = to_calc() * static_cast<CalcType>(rhs);
        *this = uint_t(res & ((CalcType(1) << Bits) - 1));
        return *this;
    }

    template<class T>
    constexpr DerivedType& operator/=(const T& rhs) noexcept {
        CalcType res = to_calc() / static_cast<CalcType>(rhs);
        *this = uint_t(res);
        return *this;
    }

    template<class T>
    constexpr DerivedType& operator%=(const T& rhs) noexcept {
        CalcType res = to_calc() % static_cast<CalcType>(rhs);
        *this = uint_t(res);
        return *this;
    }

    template<class T>
    constexpr DerivedType& operator&=(const T& rhs) noexcept {
        if constexpr(std::is_same_v<std::decay_t<T>, uint_t<Bits>>) {
            low &= rhs.low;
            if constexpr(!std::is_void_v<HighType>) high &= rhs.high;
        }
        else if constexpr(is_uint_t_v<T>) {
            CalcType val = static_cast<CalcType>(rhs);
            *this = uint_t(to_calc() & val);
        }
        else if constexpr(std::is_integral_v<T>) {
            low &= static_cast<LowType>(rhs);
            if constexpr(!std::is_void_v<HighType>)
                high &= static_cast<HighType>(rhs >> picker::LowBits);
        }
        else { static_assert(true, "Unsupported type"); }
        return *this;
    }

    template<class T>
    constexpr DerivedType& operator|=(const T& rhs) noexcept {
        if constexpr(std::is_same_v<std::decay_t<T>, uint_t<Bits>>) {
            low |= rhs.low;
            if constexpr(!std::is_void_v<HighType>) high |= rhs.high;
        }
        else if constexpr(is_uint_t_v<T>) {
            CalcType val = static_cast<CalcType>(rhs);
            *this = uint_t(to_calc() | val);
        }
        else if constexpr(std::is_integral_v<T>) {
            low |= static_cast<LowType>(rhs);
            if constexpr(!std::is_void_v<HighType>)
                high |= static_cast<HighType>(rhs >> picker::LowBits);
        }
        else { static_assert(true, "Unsupported type"); }
        return *this;
    }

    template<class T>
    constexpr DerivedType& operator^=(const T& rhs) noexcept {
        if constexpr(std::is_same_v<std::decay_t<T>, uint_t<Bits>>) {
            low ^= rhs.low;
            if constexpr(!std::is_void_v<HighType>) high ^= rhs.high;
        }
        else if constexpr(is_uint_t_v<T>) {
            CalcType val = static_cast<CalcType>(rhs);
            *this = uint_t(to_calc() ^ val);
        }
        else if constexpr(std::is_integral_v<T>) {
            low ^= static_cast<LowType>(rhs);
            if constexpr(!std::is_void_v<HighType>)
                high ^= static_cast<HighType>(rhs >> picker::LowBits);
        }
        else { static_assert(true, "Unsupported type"); }
        return *this;
    }

    constexpr DerivedType& operator<<=(int shift) noexcept {
        CalcType res = this->to_calc() << shift;
        *this = uint_t(res & ((CalcType(1) << Bits) - 1));
        return *this;
    }

    constexpr DerivedType& operator>>=(int shift) noexcept {
        CalcType res = this->to_calc() >> shift;
        *this = uint_t(res);
        return *this;
    }

    template<typename L, typename R>
    requires(!is_int_t_v<L> || !is_int_t_v<R>)
    friend DerivedType operator+(const L& lhs, const R& rhs) noexcept {
        uint_t result(lhs);
        result += rhs;
        return result;
    }

    template<int LBits, int RBits>
    friend constexpr auto operator+(const uint_t<LBits>& lhs, const uint_t<RBits>& rhs) noexcept;

    template<typename L, typename R>
    requires(!is_int_t_v<L> || !is_int_t_v<R>)
    friend DerivedType operator-(const L& lhs, const R& rhs) noexcept {
        uint_t result(lhs);
        result -= rhs;
        return result;
    }

    template<int LBits, int RBits>
    friend constexpr auto operator-(const uint_t<LBits>& lhs, const uint_t<RBits>& rhs) noexcept;

    template<typename L, typename R>
    requires(!is_int_t_v<L> || !is_int_t_v<R>)
    friend DerivedType operator*(const L& lhs, const R& rhs) noexcept {
        uint_t result(lhs);
        result *= rhs;
        return result;
    }

    template<int LBits, int RBits>
    friend constexpr auto operator*(const uint_t<LBits>& lhs, const uint_t<RBits>& rhs) noexcept;

    template<typename L, typename R>
    requires(!is_int_t_v<L> || !is_int_t_v<R>)
    friend DerivedType operator/(const L& lhs, const R& rhs) noexcept {
        uint_t result(lhs);
        result /= rhs;
        return result;
    }

    template<int LBits, int RBits>
    friend constexpr auto operator/(const uint_t<LBits>& lhs, const uint_t<RBits>& rhs) noexcept;

    template<typename L, typename R>
    requires(!is_int_t_v<L> || !is_int_t_v<R>)
    friend DerivedType operator%(const L& lhs, const R& rhs) noexcept {
        uint_t result(lhs);
        result %= rhs;
        return result;
    }

    template<int LBits, int RBits>
    friend constexpr auto operator%(const uint_t<LBits>& lhs, const uint_t<RBits>& rhs) noexcept;

    template<typename L, typename R>
    requires(!is_int_t_v<L> || !is_int_t_v<R>)
    friend DerivedType operator&(const L& lhs, const R& rhs) noexcept {
        uint_t result(lhs);
        result &= rhs;
        return result;
    }

    template<int LBits, int RBits>
    friend constexpr auto operator&(const uint_t<LBits>& lhs, const uint_t<RBits>& rhs) noexcept;

    template<typename L, typename R>
    requires(!is_int_t_v<L> || !is_int_t_v<R>)
    friend DerivedType operator|(const L& lhs, const R& rhs) noexcept {
        uint_t result(lhs);
        result |= rhs;
        return result;
    }

    template<int LBits, int RBits>
    friend constexpr auto operator|(const uint_t<LBits>& lhs, const uint_t<RBits>& rhs) noexcept;

    template<typename L, typename R>
    requires(!is_int_t_v<L> && !is_int_t_v<R>)
    friend DerivedType operator^(const L& lhs, const R& rhs) noexcept {
        uint_t result(lhs);
        result ^= rhs;
        return result;
    }

    template<int LBits, int RBits>
    friend constexpr auto operator^(const uint_t<LBits>& lhs, const uint_t<RBits>& rhs) noexcept;

    friend constexpr DerivedType operator<<(uint_t lhs, int shift) noexcept {
        lhs <<= shift;
        return lhs;
    }

    friend constexpr DerivedType operator>>(uint_t lhs, int shift) noexcept {
        lhs >>= shift;
        return lhs;
    }

    friend constexpr auto operator<=>(const uint_t& lhs, const uint_t& rhs) noexcept {
        if constexpr(std::is_void_v<HighType>) return lhs.low <=> rhs.low;
        else return (lhs.high != rhs.high ? lhs.high <=> rhs.high : lhs.low <=> rhs.low);
    }

    template<class L, class R>
    requires(0 < Bits)
    friend constexpr auto operator<=>(const L& lhs, const R& rhs) noexcept {
        using CalcType = typename uint_t::CalcType;
        return static_cast<CalcType>(lhs) <=> static_cast<CalcType>(rhs);
    }

    template<int LBits, int RBits>
    friend constexpr auto operator<=>(const uint_t<LBits>& lhs, const uint_t<RBits>& rhs) noexcept;

    friend constexpr bool operator==(const uint_t& lhs, const uint_t& rhs) noexcept {
        if constexpr(std::is_void_v<HighType>) return lhs.low == rhs.low;
        else return lhs.low == rhs.low && lhs.high == rhs.high;
    }

    template<class L, class R>
    requires(0 < Bits)
    friend constexpr bool operator==(const L& lhs, const R& rhs) noexcept {
        using CalcType = typename uint_t::CalcType;
        return static_cast<CalcType>(lhs) == static_cast<CalcType>(rhs);
    }

    template<int LBits, int RBits>
    friend constexpr bool operator==(const uint_t<LBits>& lhs, const uint_t<RBits>& rhs) noexcept;

    friend std::istream& operator>>(std::istream& is, uint_t& v) {
        using CalcType = typename uint_t::CalcType;
        CalcType tmp{};
        is >> tmp;
        v = tmp;
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const uint_t& v) { return os << v.to_calc(); }
};
#pragma pack(pop)

template<typename L, typename R>
struct result_type_picker;

template<int LBits, typename LDer, int RBits, typename RDer>
struct result_type_picker<uint_t<LBits, LDer>, uint_t<RBits, RDer>> {
    static constexpr int ResultBits = (LBits > RBits) ? LBits : RBits;
    using type = std::conditional_t<
        !std::is_void_v<LDer> || !std::is_void_v<RDer>,
        int_t<ResultBits>, uint_t<ResultBits>>;
};

template<typename L, typename R>
using result_type_t = typename result_type_picker<L, R>::type;

template<int LBits, typename LDer, int RBits, typename RDer>
constexpr auto operator+(const uint_t<LBits, LDer>& lhs, const uint_t<RBits, RDer>& rhs) noexcept {
    using ResultType = result_type_t<uint_t<LBits, LDer>, uint_t<RBits, RDer>>;
    ResultType result(lhs);
    result += rhs;
    return result;
}

template<int LBits, typename LDer, int RBits, typename RDer>
constexpr auto operator-(const uint_t<LBits, LDer>& lhs, const uint_t<RBits, RDer>& rhs) noexcept {
    using ResultType = result_type_t<uint_t<LBits, LDer>, uint_t<RBits, RDer>>;
    ResultType result(lhs);
    result -= rhs;
    return result;
}

template<int LBits, typename LDer, int RBits, typename RDer>
constexpr auto operator*(const uint_t<LBits, LDer>& lhs, const uint_t<RBits, RDer>& rhs) noexcept {
    using ResultType = result_type_t<uint_t<LBits, LDer>, uint_t<RBits, RDer>>;
    ResultType result(lhs);
    result *= rhs;
    return result;
}

template<int LBits, typename LDer, int RBits, typename RDer>
constexpr auto operator/(const uint_t<LBits, LDer>& lhs, const uint_t<RBits, RDer>& rhs) noexcept {
    using ResultType = result_type_t<uint_t<LBits, LDer>, uint_t<RBits, RDer>>;
    ResultType result(lhs);
    result /= rhs;
    return result;
}

template<int LBits, typename LDer, int RBits, typename RDer>
constexpr auto operator%(const uint_t<LBits, LDer>& lhs, const uint_t<RBits, RDer>& rhs) noexcept {
    using ResultType = result_type_t<uint_t<LBits, LDer>, uint_t<RBits, RDer>>;
    ResultType result(lhs);
    result %= rhs;
    return result;
}

template<int LBits, typename LDer, int RBits, typename RDer>
constexpr auto operator&(const uint_t<LBits, LDer>& lhs, const uint_t<RBits, RDer>& rhs) noexcept {
    using ResultType = result_type_t<uint_t<LBits, LDer>, uint_t<RBits, RDer>>;
    ResultType result(lhs);
    result &= rhs;
    return result;
}

template<int LBits, typename LDer, int RBits, typename RDer>
constexpr auto operator|(const uint_t<LBits, LDer>& lhs, const uint_t<RBits, RDer>& rhs) noexcept {
    using ResultType = result_type_t<uint_t<LBits, LDer>, uint_t<RBits, RDer>>;
    ResultType result(lhs);
    result |= rhs;
    return result;
}

template<int LBits, typename LDer, int RBits, typename RDer>
constexpr auto operator^(const uint_t<LBits, LDer>& lhs, const uint_t<RBits, RDer>& rhs) noexcept {
    using ResultType = result_type_t<uint_t<LBits, LDer>, uint_t<RBits, RDer>>;
    ResultType result(lhs);
    result ^= rhs;
    return result;
}

template<int LBits, int RBits>
constexpr auto operator<=>(const uint_t<LBits>& lhs, const uint_t<RBits>& rhs) noexcept {
    constexpr int ResultBits = (LBits > RBits) ? LBits : RBits;
    using CalcType = uint_t<ResultBits>::CalcType;
    return static_cast<CalcType>(lhs) <=> static_cast<CalcType>(rhs);
}

template<int LBits, int RBits>
constexpr bool operator==(const uint_t<LBits>& lhs, const uint_t<RBits>& rhs) noexcept {
    constexpr int ResultBits = (LBits > RBits) ? LBits : RBits;
    using CalcType = uint_t<ResultBits>::CalcType;
    return static_cast<CalcType>(lhs) == static_cast<CalcType>(rhs);
}

using uint24_t = uint_t<24>;
using uint40_t = uint_t<40>;
using uint48_t = uint_t<48>;
using uint72_t = uint_t<72>;
using uint80_t = uint_t<80>;
using uint96_t = uint_t<96>;

}// namespace kusu_lib

namespace std {

template<int Bits>
struct is_integral<kusu_lib::uint_t<Bits>> : true_type {};

template<int Bits>
struct numeric_limits<kusu_lib::uint_t<Bits>> {
    static constexpr bool is_specialized = true;
    static constexpr bool is_integer = true;
    static constexpr bool is_signed = false;
    static constexpr bool is_exact = true;
    static constexpr bool is_bounded = true;
    static constexpr bool is_modulo = true;

    static constexpr kusu_lib::uint_t<Bits> min() noexcept { return kusu_lib::uint_t<Bits>(0); }
    static constexpr kusu_lib::uint_t<Bits> lowest() noexcept { return min(); }
    static constexpr kusu_lib::uint_t<Bits> max() noexcept {
        using CT = typename kusu_lib::uint_type_picker<Bits>::CalcType;
        return kusu_lib::uint_t<Bits>((Bits == 128) ? ~CT(0) : ((CT(1) << Bits) - 1));
    }
};
}// namespace std
