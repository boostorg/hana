/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/variadic/reverse_apply.hpp>
#include <boost/hana/detail/variadic/reverse_apply/flat.hpp>
#include <boost/hana/detail/variadic/reverse_apply/unrolled.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
using namespace boost::hana;


auto check = [](auto reverse_apply) {
    using test::x;
    auto f = test::injection([]{});

    BOOST_HANA_CONSTANT_ASSERT(equal(
        reverse_apply(f),
        f()
    ));
    BOOST_HANA_CONSTANT_ASSERT(equal(
        reverse_apply(f, x<0>),
        f(x<0>)
    ));
    BOOST_HANA_CONSTANT_ASSERT(equal(
        reverse_apply(f, x<0>, x<1>),
        f(x<1>, x<0>)
    ));
    BOOST_HANA_CONSTANT_ASSERT(equal(
        reverse_apply(f, x<0>, x<1>, x<2>),
        f(x<2>, x<1>, x<0>)
    ));
    BOOST_HANA_CONSTANT_ASSERT(equal(
        reverse_apply(f, x<0>, x<1>, x<2>, x<3>),
        f(x<3>, x<2>, x<1>, x<0>)
    ));
    BOOST_HANA_CONSTANT_ASSERT(equal(
        reverse_apply(f, x<0>, x<1>, x<2>, x<3>, x<4>),
        f(x<4>, x<3>, x<2>, x<1>, x<0>)
    ));
    BOOST_HANA_CONSTANT_ASSERT(equal(
        reverse_apply(f, x<0>, x<1>, x<2>, x<3>, x<4>, x<5>),
        f(x<5>, x<4>, x<3>, x<2>, x<1>, x<0>)
    ));
    BOOST_HANA_CONSTANT_ASSERT(equal(
        reverse_apply(f, x<0>, x<1>, x<2>, x<3>, x<4>, x<5>, x<6>),
        f(x<6>, x<5>, x<4>, x<3>, x<2>, x<1>, x<0>)
    ));
    BOOST_HANA_CONSTANT_ASSERT(equal(
        reverse_apply(f, x<0>, x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>),
        f(x<7>, x<6>, x<5>, x<4>, x<3>, x<2>, x<1>, x<0>)
    ));
    BOOST_HANA_CONSTANT_ASSERT(equal(
        reverse_apply(f, x<0>, x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>),
        f(x<8>, x<7>, x<6>, x<5>, x<4>, x<3>, x<2>, x<1>, x<0>)
    ));
};

int main() {
    check(detail::variadic::reverse_apply);
    check([](auto f, auto ...x) {
        return detail::variadic::reverse_apply_flat(f, x...);
    });
    check([](auto f, auto ...x) {
        return detail::variadic::reverse_apply_unrolled(f, x...);
    });
}
