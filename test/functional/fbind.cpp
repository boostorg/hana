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


int main() {
    BOOST_HANA_STATIC_ASSERT(fbind(list)() == list());
    BOOST_HANA_STATIC_ASSERT(fbind(list, _ + int_<1>)(int_<0>) == list_c<int, 1>);
    BOOST_HANA_STATIC_ASSERT(fbind(list, _ + int_<1>, _ + int_<2>)(int_<0>) == list_c<int, 1, 2>);
    BOOST_HANA_STATIC_ASSERT(fbind(list, _ + _, _ * _)(int_<2>, int_<3>) == list_c<int, 2 + 3, 2 * 3>);
}
