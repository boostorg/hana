/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/variadic/foldl.hpp>

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

struct S;
struct x1; struct x2; struct x3; struct x4; struct x5; struct x6;

template <typename ...Ts, typename S, typename R>
void test_impl(S s, R result) {
    BOOST_HANA_CONSTANT_ASSERT(variadic::foldl<Ts...>(mf, s) == result);
    BOOST_HANA_CONSTANT_ASSERT(variadic::foldl<Ts...>(f, s) == result);
    BOOST_HANA_CONSTANT_ASSERT(variadic::foldl(mf, s, type<Ts>...) == result);
    BOOST_HANA_CONSTANT_ASSERT(variadic::foldl(f, s, type<Ts>...) == result);
}

template <typename ...Ts, typename R>
void test(R result) {
    test_impl<Ts...>(type<S>, result);

    struct not_a_Type { using type = S; };
    test_impl<Ts...>(not_a_Type{}, result);
}

int main() {
    test_impl<>(type<S>, type<S>);

    test<x1>(type<R<S, x1>>);
    test<x1, x2>(type<R<R<S, x1>, x2>>);
    test<x1, x2, x3>(type<R<R<R<S, x1>, x2>, x3>>);
    test<x1, x2, x3, x4>(type<R<R<R<R<S, x1>, x2>, x3>, x4>>);
    test<x1, x2, x3, x4, x5>(type<R<R<R<R<R<S, x1>, x2>, x3>, x4>, x5>>);
    test<x1, x2, x3, x4, x5, x6>(type<R<R<R<R<R<R<S, x1>, x2>, x3>, x4>, x5>, x6>>);

    (void)f;
}
