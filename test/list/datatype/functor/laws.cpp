/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functor/laws.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto i) { return i + int_<1>; };
BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto i) { return i * int_<3>; };

int main() {
    BOOST_HANA_STATIC_ASSERT(Functor::laws::check(list(), f, g));
    BOOST_HANA_STATIC_ASSERT(Functor::laws::check(list(int_<1>), f, g));
    BOOST_HANA_STATIC_ASSERT(Functor::laws::check(list(int_<1>, int_<2>), f, g));
    BOOST_HANA_STATIC_ASSERT(Functor::laws::check(list(1, 2, 3), f, g));
}
