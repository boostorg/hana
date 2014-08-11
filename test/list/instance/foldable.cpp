/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/instance.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});

template <int i>
constexpr auto x = detail::number<>(i);

int main() {
    // unpack
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(unpack(list(), f), f()));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(unpack(list(x<0>), f), f(x<0>)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(unpack(list(x<0>, x<1>), f), f(x<0>, x<1>)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(unpack(list(x<0>, x<1>, x<2>), f), f(x<0>, x<1>, x<2>)));
    }
}
