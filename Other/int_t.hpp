#pragma once
#include <bits/stdc++.h>
#include "int128.hpp"
#include "uint_t.hpp"// あなたの uint_t 実装をこのヘッダ名で読み込む想定

namespace kusu_lib {
#pragma pack(push, 1)

template<int Bits>
struct int_type_picker {
    static_assert(0 < Bits && Bits <= 128);

    // reuse uint_type_picker to pick Low/High/Calc unsigned types, then make signed variant when
    // needed.
    using upicker = uint_type_picker<Bits>;
    static constexpr int LowBits = upicker::LowBits;
    static constexpr int HighBits = upicker::HighBits;

    using ULowType = typename upicker::LowType;
    using UHighType = typename upicker::HighType;
    using UCalcType = typename upicker::CalcType;

    using LowType = std::make_signed_t<ULowType>;
    using HighType =
        std::conditional_t<std::is_void_v<UHighType>, void, std::make_signed_t<UHighType>>;
    using CalcType = std::make_signed_t<UCalcType>;
    using UCalcType_t = UCalcType;
};

//is_int_t is defined in uint_t.hpp

template<int Bits>
class int_t : public uint_t<Bits,int_t<Bits>> {
    static_assert(0 < Bits && Bits <= 128);
    using base = uint_t<Bits,int_t<Bits>>;
    using CalcType = typename uint_type_picker<Bits>::CalcType;
    using SCalc = std::make_signed_t<CalcType>;

    constexpr SCalc to_scalc() const noexcept {
        if constexpr(std::is_void_v<typename base::HighType>) return static_cast<SCalc>(this->low);
        else {
            if((this->high >> (Bits - 1 - uint_type_picker<Bits>::LowBits)) & 1)
                return -static_cast<SCalc>((static_cast<CalcType>(1) << Bits) - this->to_calc());
            else return static_cast<SCalc>(this->to_calc());
        }
    }

  public:
    using base::base;

    constexpr int_t(uint_t<Bits> val) noexcept : base(val) {}

    friend std::istream& operator>>(std::istream& is, int_t& v) {
        using SCalc = typename int_t::SCalc;
        SCalc tmp{};
        is >> tmp;
        v = tmp;
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const int_t& v) {
        auto a = v.to_scalc();
        return os << a;
    }
};
#pragma pack(pop)

using int24_t = int_t<24>;
using int40_t = int_t<40>;
using int48_t = int_t<48>;
using int72_t = int_t<72>;
using int80_t = int_t<80>;
using int96_t = int_t<96>;

}// namespace kusu_lib

namespace std {
template<int Bits>
struct is_integral<kusu_lib::int_t<Bits>> : true_type {};

template<int Bits>
struct numeric_limits<kusu_lib::int_t<Bits>> {
    static constexpr bool is_specialized = true;
    static constexpr bool is_integer = true;
    static constexpr bool is_signed = true;
    static constexpr bool is_exact = true;
    static constexpr bool is_bounded = true;
    static constexpr bool is_modulo = true;

    using T = kusu_lib::int_t<Bits>;
    using CT = typename kusu_lib::int_type_picker<Bits>::CalcType;

    static constexpr T min() noexcept {
        CT v = CT(1) << (Bits - 1);
        return T(v);
    }
    static constexpr T lowest() noexcept { return min(); }
    static constexpr T max() noexcept {
        CT v = (CT(1) << (Bits - 1)) - 1;
        return T(v);
    }
};

}// namespace std
