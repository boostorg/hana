/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include "../foldable/minimal_foldable.hpp"
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(to<List>(foldable()) == list());
    BOOST_HANA_STATIC_ASSERT(to<List>(foldable(int_<0>)) == list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(to<List>(foldable(int_<0>, int_<1>)) == list(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(to<List>(foldable(int_<0>, int_<1>, int_<2>)) == list(int_<0>, int_<1>, int_<2>));
    BOOST_HANA_STATIC_ASSERT(to<List>(foldable(int_<0>, int_<1>, int_<2>, int_<3>)) == list(int_<0>, int_<1>, int_<2>, int_<3>));

    BOOST_HANA_STATIC_ASSERT(to<List>(foldable(0)) == list(0));
    BOOST_HANA_STATIC_ASSERT(to<List>(foldable(0, '1')) == list(0, '1'));
    BOOST_HANA_STATIC_ASSERT(to<List>(foldable(0, '1', 2.2)) == list(0, '1', 2.2));
    BOOST_HANA_STATIC_ASSERT(to<List>(foldable(0, '1', 2.2, 3.3f)) == list(0, '1', 2.2, 3.3f));
}
