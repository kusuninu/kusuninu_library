#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {

template<class S, auto op, auto e>
concept Monoid = std::copyable<S> && requires(S a, S b) {
    { op(a, b) } -> std::same_as<S>;
    { e() } -> std::same_as<S>;
};

template<class S, class F, auto mapping, auto composition, auto id>
concept SegMapping = std::copyable<S> && std::copyable<F> && requires(S a, F f, F g) {
    { mapping(f,a) } -> std::same_as<S>;
    { composition(f, g) } -> std::same_as<F>;
    { id() } -> std::same_as<F>;
};

template<class S, auto op, auto e, class F, auto mapping, auto composition, auto id>
concept LazySegMonoidMapping = Monoid<S, op, e> && SegMapping<S, F, mapping, composition, id>;

}//namespace kusu_lib