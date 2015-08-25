/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/not_equal.hpp>
#include <boost/hana/type.hpp>
namespace hana = boost::hana;


struct T;
struct U;
BOOST_HANA_CONSTANT_CHECK(hana::type<T> == hana::type<T>);
BOOST_HANA_CONSTANT_CHECK(hana::type<T> != hana::type<U>);

int main() { }
