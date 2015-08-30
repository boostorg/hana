/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/type.hpp>
namespace hana = boost::hana;


struct X { };
BOOST_HANA_CONSTANT_CHECK(hana::make<hana::Type>(X{}) == hana::decltype_(X{}));
BOOST_HANA_CONSTANT_CHECK(hana::make<hana::Type>(hana::type_c<X>) == hana::decltype_(hana::type_c<X>));

int main() { }
