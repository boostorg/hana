/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


int main() {
    static_assert(hana::to<hana::Tuple>(hana::just(1)) == hana::make_tuple(1), "");
    BOOST_HANA_CONSTANT_CHECK(hana::to<hana::Tuple>(hana::nothing) == hana::make_tuple());

    BOOST_HANA_CONSTANT_CHECK(
        hana::to<hana::Tuple>(hana::make_range(hana::int_<3>, hana::int_<6>))
            ==
        hana::tuple_c<int, 3, 4, 5>
    );
}
