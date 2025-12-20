#pragma once
#include <bits/stdc++.h>
#include "int128.hpp"

namespace kusu_lib {

//---------------------------------------------
// 共通基礎型の選択
//---------------------------------------------
template<int Bits>
struct uint_type_picker {
    static_assert(0 < Bits && Bits <= 128);

    static constexpr int LowBits = (64 <= Bits ? 64 : (32 <= Bits ? 32 : (16 <= Bits ? 16 : 8)));
    static constexpr int HighBits = Bits - LowBits;

    using LowType = std::conditional_t<
        LowBits <= 8,  uint8_t,
        std::conditional_t<
            LowBits <= 16, uint16_t,
            std::conditional_t<
                LowBits <= 32, uint32_t,
                uint64_t>>>;

    using HighType = std::conditional_t<
        HighBits == 0, void,
        std::conditional_t<
            HighBits <= 8,  uint8_t,
            std::conditional_t<
                HighBits <= 16, uint16_t,
                std::conditional_t<
                    HighBits <= 32, uint32_t,
                    uint64_t>>>>;

    using CalcType = std::conditional_t<
        Bits <= 8,  uint8_t,
        std::conditional_t<
            Bits <= 16, uint16_t,
            std::conditional_t<
                Bits <= 32, uint32_t,
                std::conditional_t<
                    Bits <= 64, uint64_t,
                    __uint128_t>>>>;
};

//---------------------------------------------
// 共通基底クラス: uint_t_base
//---------------------------------------------
template<int Bits, class Derived>
class uint_t_base {
protected:
    using picker    = uint_type_picker<Bits>;
    using LowType   = typename picker::LowType;
    using HighType  = typename picker::HighType;
    using CalcType  = typename picker::CalcType;
    using DerivedType = Derived;

    LowType low{};
    std::conditional_t<std::is_void_v<HighType>, char, HighType> high{};

public:
    constexpr uint_t_base() noexcept = default;

    // integral から構築
    template<std::integral T>
    constexpr uint_t_base(T val) noexcept {
        using U = std::make_unsigned_t<CalcType>;
        U uval = static_cast<U>(val);
        low = static_cast<LowType>(uval);
        if constexpr(!std::is_void_v<HighType>)
            high = static_cast<HighType>(uval >> picker::LowBits);
    }

    // CalcType変換
    constexpr CalcType to_calc() const noexcept {
        if constexpr(!std::is_void_v<HighType>)
            return (static_cast<CalcType>(high) << picker::LowBits) | low;
        else return static_cast<CalcType>(low);
    }

    // 基本演算
    constexpr DerivedType& operator+=(const DerivedType& rhs) noexcept {
        if constexpr(std::is_void_v<HighType>) {
            low += rhs.low;
        } else {
            LowType old_low = low;
            low += rhs.low;
            if(low < old_low) ++high;
            high += rhs.high;
        }
        return static_cast<DerivedType&>(*this);
    }

    constexpr DerivedType& operator-=(const DerivedType& rhs) noexcept {
        if constexpr(std::is_void_v<HighType>) {
            low -= rhs.low;
        } else {
            LowType old_low = low;
            low -= rhs.low;
            if(low > old_low) --high;
            high -= rhs.high;
        }
        return static_cast<DerivedType&>(*this);
    }

    constexpr DerivedType& operator*=(const DerivedType& rhs) noexcept {
        CalcType res = to_calc() * rhs.to_calc();
        *this = DerivedType(res);
        return static_cast<DerivedType&>(*this);
    }

    constexpr DerivedType& operator/=(const DerivedType& rhs) noexcept {
        CalcType res = to_calc() / rhs.to_calc();
        *this = DerivedType(res);
        return static_cast<DerivedType&>(*this);
    }

    constexpr DerivedType& operator%=(const DerivedType& rhs) noexcept {
        CalcType res = to_calc() % rhs.to_calc();
        *this = DerivedType(res);
        return static_cast<DerivedType&>(*this);
    }

    // ビット演算
    constexpr DerivedType& operator&=(const DerivedType& rhs) noexcept {
        low &= rhs.low;
        if constexpr(!std::is_void_v<HighType>) high &= rhs.high;
        return static_cast<DerivedType&>(*this);
    }

    constexpr DerivedType& operator|=(const DerivedType& rhs) noexcept {
        low |= rhs.low;
        if constexpr(!std::is_void_v<HighType>) high |= rhs.high;
        return static_cast<DerivedType&>(*this);
    }

    constexpr DerivedType& operator^=(const DerivedType& rhs) noexcept {
        low ^= rhs.low;
        if constexpr(!std::is_void_v<HighType>) high ^= rhs.high;
        return static_cast<DerivedType&>(*this);
    }

    // シフト演算
    constexpr DerivedType& operator<<=(int shift) noexcept {
        *this = DerivedType(to_calc() << shift);
        return static_cast<DerivedType&>(*this);
    }

    constexpr DerivedType& operator>>=(int shift) noexcept {
        *this = DerivedType(to_calc() >> shift);
        return static_cast<DerivedType&>(*this);
    }

    // 比較
    constexpr auto operator<=>(const DerivedType& rhs) const noexcept = default;
    constexpr bool operator==(const DerivedType& rhs) const noexcept = default;

    // 変換
    constexpr operator CalcType() const noexcept { return to_calc(); }

    // 出力
    friend std::ostream& operator<<(std::ostream& os, const uint_t_base& v) {
        return os << v.to_calc();
    }
};

} // namespace kusu_lib
