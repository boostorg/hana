/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/erase_key.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/type.hpp>
namespace hana = boost::hana;


int main() {
    constexpr auto xs = hana::make_set(hana::int_<0>, hana::type<int>, hana::type<void>);

    BOOST_HANA_CONSTANT_CHECK(hana::erase_key(xs, hana::type<int>) == hana::make_set(hana::int_<0>, hana::type<void>));
    BOOST_HANA_CONSTANT_CHECK(hana::erase_key(xs, hana::type<char>) == xs);
}
