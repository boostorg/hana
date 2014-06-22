/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


template <char op>
BOOST_HANA_CONSTEXPR_LAMBDA auto function = nothing;

template <>
BOOST_HANA_CONSTEXPR_LAMBDA auto function<'+'> = just([](auto x, auto y) {
    return x + y;
});

template <>
BOOST_HANA_CONSTEXPR_LAMBDA auto function<'-'> = just([](auto x, auto y) {
    return x - y;
});

// and so on...

template <char n>
BOOST_HANA_CONSTEXPR_LAMBDA auto digit = if_(bool_<(n >= '0' && n <= '9')>,
    just(static_cast<int>(n - 48)), nothing
);

template <char x, char op, char y>
BOOST_HANA_CONSTEXPR_LAMBDA auto eval = ap(ap(fmap(curry<2>, function<op>), digit<x>), digit<y>);

int main() {
    BOOST_HANA_STATIC_ASSERT(eval<'1', '+', '2'> == just(1 + 2));

    BOOST_HANA_STATIC_ASSERT(eval<'?', '+', '2'> == nothing);
    BOOST_HANA_STATIC_ASSERT(eval<'1', '?', '2'> == nothing);
    BOOST_HANA_STATIC_ASSERT(eval<'1', '+', '?'> == nothing);
    BOOST_HANA_STATIC_ASSERT(eval<'?', '?', '?'> == nothing);

    BOOST_HANA_STATIC_ASSERT(lift<Maybe>(123) == just(123));
}
