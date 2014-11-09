/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/variadic/foldr.hpp>

#include <boost/hana/assert.hpp>
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
    BOOST_HANA_CONSTANT_CHECK(equal(variadic::foldr_t<Ts...>(mf, s), result));
    BOOST_HANA_CONSTANT_CHECK(equal(variadic::foldr_t<Ts...>(f, s), result));
    BOOST_HANA_CONSTANT_CHECK(equal(variadic::foldr(mf, s, type<Ts>...), result));
    BOOST_HANA_CONSTANT_CHECK(equal(variadic::foldr(f, s, type<Ts>...), result));
}

template <typename ...Ts, typename R>
void test(R result) {
    test_impl<Ts...>(type<S>, result);

    struct not_a_Type { using type = S; };
    test_impl<Ts...>(not_a_Type{}, result);
}

int main() {
    test_impl<>(type<S>, type<S>);

    test<x1>(type<R<x1, S>>);
    test<x1, x2>(type<R<x1, R<x2, S>>>);
    test<x1, x2, x3>(type<R<x1, R<x2, R<x3, S>>>>);
    test<x1, x2, x3, x4>(type<R<x1, R<x2, R<x3, R<x4, S>>>>>);
    test<x1, x2, x3, x4, x5>(type<R<x1, R<x2, R<x3, R<x4, R<x5, S>>>>>>);
    test<x1, x2, x3, x4, x5, x6>(type<R<x1, R<x2, R<x3, R<x4, R<x5, R<x6, S>>>>>>>);

    (void)f;
}
