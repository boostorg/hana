/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto inc = [](auto x) { return x + int_<1>; };

int main() {
    BOOST_HANA_STATIC_ASSERT(on(list, inc)() == list());
    BOOST_HANA_STATIC_ASSERT(on(list, inc)(int_<0>) == list_c<int, 1>);
    BOOST_HANA_STATIC_ASSERT(on(list, inc)(int_<0>, int_<1>) == list_c<int, 1, 2>);
    BOOST_HANA_STATIC_ASSERT(on(list, inc)(int_<0>, int_<1>, int_<2>) == list_c<int, 1, 2, 3>);
    BOOST_HANA_STATIC_ASSERT(on(list, inc)(0, 1, 2, 3) == list(1, 2, 3, 4));
}
