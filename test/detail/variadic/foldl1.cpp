/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/variadic/foldl1.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;
namespace variadic = detail::variadic;


template <typename X, typename Y>
struct R;

template <typename X, typename Y>
struct F { using type = R<X, Y>; };

BOOST_HANA_CONSTEXPR_LAMBDA auto mf = metafunction<F>;
BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x, auto y) { return mf(x, y); };

template <typename ...Ts, typename R>
void test(R result) {
    static_assert(sizeof...(Ts) >= 1, "foldl1 requires at least 1 element");
    BOOST_HANA_CONSTANT_ASSERT(variadic::foldl1<Ts...>(mf) == result);
    BOOST_HANA_CONSTANT_ASSERT(variadic::foldl1<Ts...>(f) == result);
    BOOST_HANA_CONSTANT_ASSERT(variadic::foldl1(mf, type<Ts>...) == result);
    BOOST_HANA_CONSTANT_ASSERT(variadic::foldl1(f, type<Ts>...) == result);
}

struct x1; struct x2; struct x3; struct x4; struct x5; struct x6;

int main() {
    test<x1>(type<x1>);
    test<x1, x2>(type<R<x1, x2>>);
    test<x1, x2, x3>(type<R<R<x1, x2>, x3>>);
    test<x1, x2, x3, x4>(type<R<R<R<x1, x2>, x3>, x4>>);
    test<x1, x2, x3, x4, x5>(type<R<R<R<R<x1, x2>, x3>, x4>, x5>>);
    test<x1, x2, x3, x4, x5, x6>(type<R<R<R<R<R<x1, x2>, x3>, x4>, x5>, x6>>);

    (void)f;
}
