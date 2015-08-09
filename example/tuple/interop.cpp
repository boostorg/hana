/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/ext/std/array.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>

#include <array>
#include <tuple>
namespace hana = boost::hana;


int main() {
    static_assert(
        hana::to<hana::Tuple>(std::make_tuple(1, '2', 3.3)) == hana::make_tuple(1, '2', 3.3)
    , "");

    BOOST_HANA_CONSTANT_CHECK(
        hana::to<hana::Tuple>(hana::make_range(hana::int_<1>, hana::int_<4>))
            ==
        hana::make_tuple(hana::int_<1>, hana::int_<2>, hana::int_<3>)
    );

    // std::array's operator[] is not constexpr, so we can't use static_assert
    BOOST_HANA_CONSTEXPR_CHECK(
        hana::to<hana::Tuple>(std::array<int, 3>{{1, 2, 3}}) == hana::make_tuple(1, 2, 3)
    );
}