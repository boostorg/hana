/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/remove.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
namespace hana = boost::hana;


BOOST_HANA_CONSTANT_CHECK(hana::remove(hana::tuple_t<int, char, float>, hana::type<char>) == hana::tuple_t<int, float>);
BOOST_HANA_CONSTANT_CHECK(hana::remove(hana::just(hana::type<int>), hana::type<char>) == hana::just(hana::type<int>));
BOOST_HANA_CONSTANT_CHECK(hana::remove(hana::just(hana::type<int>), hana::type<int>) == hana::nothing);

int main() { }
