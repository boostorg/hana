/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/set.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = int_<i>;

int main() {
    constexpr auto list = detail::minimal::list<>;

    BOOST_HANA_CONSTANT_ASSERT(to<Set>(list()) == set());
    BOOST_HANA_CONSTANT_ASSERT(to<Set>(list(x<1>)) == set(x<1>));
    BOOST_HANA_CONSTANT_ASSERT(to<Set>(list(x<1>, x<2>)) == set(x<1>, x<2>));
    BOOST_HANA_CONSTANT_ASSERT(to<Set>(list(x<1>, x<2>, x<3>)) == set(x<1>, x<2>, x<3>));
}
