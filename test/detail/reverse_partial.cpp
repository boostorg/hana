/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/reverse_partial.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/injection.hpp>
using namespace boost::hana;


constexpr auto rp = detail::reverse_partial;
BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});
using test::x;

int main() {
    BOOST_HANA_CONSTANT_ASSERT(equal(rp(f)(), f()));
    BOOST_HANA_CONSTANT_ASSERT(equal(rp(f)(x<1>), f(x<1>)));
    BOOST_HANA_CONSTANT_ASSERT(equal(rp(f)(x<1>, x<2>), f(x<1>, x<2>)));
    BOOST_HANA_CONSTANT_ASSERT(equal(rp(f)(x<1>, x<2>, x<3>), f(x<1>, x<2>, x<3>)));

    BOOST_HANA_CONSTANT_ASSERT(equal(rp(f, x<1>)(), f(x<1>)));
    BOOST_HANA_CONSTANT_ASSERT(equal(rp(f, x<1>)(x<2>), f(x<2>, x<1>)));
    BOOST_HANA_CONSTANT_ASSERT(equal(rp(f, x<1>)(x<2>, x<3>), f(x<2>, x<3>, x<1>)));

    BOOST_HANA_CONSTANT_ASSERT(equal(rp(f, x<1>, x<2>)(), f(x<1>, x<2>)));
    BOOST_HANA_CONSTANT_ASSERT(equal(rp(f, x<1>, x<2>)(x<3>), f(x<3>, x<1>, x<2>)));
    BOOST_HANA_CONSTANT_ASSERT(equal(rp(f, x<1>, x<2>)(x<3>, x<4>), f(x<3>, x<4>, x<1>, x<2>)));

    BOOST_HANA_CONSTANT_ASSERT(equal(rp(f, x<1>, x<2>, x<3>)(), f(x<1>, x<2>, x<3>)));
}
