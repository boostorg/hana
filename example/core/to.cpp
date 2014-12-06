/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/ext/std/array.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>

#include <array>
#include <tuple>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_CHECK(
        to<Tuple>(std::make_tuple(1, '2', 3.3)) == tuple(1, '2', 3.3)
    );

    BOOST_HANA_CONSTANT_CHECK(
        to<Tuple>(range(int_<1>, int_<4>)) == tuple(int_<1>, int_<2>, int_<3>)
    );

    BOOST_HANA_CONSTEXPR_CHECK(
        to<Tuple>(std::array<int, 3>{{1, 2, 3}}) == tuple(1, 2, 3)
    );
    //! [main]
}
