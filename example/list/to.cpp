/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/adapted/std_array.hpp>
#include <boost/hana/adapted/std_tuple.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/range.hpp>

#include <array>
#include <tuple>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(
        to<List>(std::make_tuple(1, '2', 3.3)) == list(1, '2', 3.3)
    );

    BOOST_HANA_STATIC_ASSERT(
        to<List>(range(int_<1>, int_<4>)) == list(int_<1>, int_<2>, int_<3>)
    );

    BOOST_HANA_STATIC_ASSERT(
        to<List>(std::array<int, 3>{{1, 2, 3}}) == list(1, 2, 3)
    );
    //! [main]
}
