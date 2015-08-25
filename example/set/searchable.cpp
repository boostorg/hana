/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/at_key.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/find.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/set.hpp>
namespace hana = boost::hana;


int main() {
    constexpr auto xs = hana::make_set(hana::int_<0>, hana::int_<1>, hana::int_<2>);
    BOOST_HANA_CONSTANT_CHECK(hana::find(xs, hana::int_<0>) == hana::just(hana::int_<0>));
    BOOST_HANA_CONSTANT_CHECK(hana::find(xs, hana::int_<3>) == hana::nothing);

    // operator[] is equivalent to at_key
    BOOST_HANA_CONSTANT_CHECK(xs[hana::int_<2>] == hana::int_<2>);

    // long_<0> == int_<0>, and therefore int_<0> is found
    BOOST_HANA_CONSTANT_CHECK(xs[hana::long_<0>] == hana::int_<0>);
}
