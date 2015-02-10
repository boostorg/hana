/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>

#include <type_traits>
using namespace boost::hana;


//! [integral_create]
auto two = integral_constant<int, 2>;
auto yes = integral_constant<bool, true>;
//! [integral_create]

//! [integral_api]
using Two = decltype(two);

static_assert(std::is_same<Two::type, Two>{}, "");

static_assert(std::is_same<Two::value_type, int>{}, "");

static_assert(Two::value == 2               &&
              static_cast<int>(two) == 2    &&
              two() == 2                    &&
              value(two) == 2
, "these are all constant expressions");
//! [integral_api]

namespace anon1 {
//! [integral_shorthands]
auto two = int_<2>;
auto yes = bool_<true>;
//! [integral_shorthands]
}

namespace anon2 {
//! [literals]
using namespace literals; // <-- mandatory to use the _c suffix!

BOOST_HANA_CONSTANT_CHECK(1234_c == llong<1234>);
BOOST_HANA_CONSTANT_CHECK(-1234_c == llong<-1234>);
//! [literals]
}

//! [integral_operators]
BOOST_HANA_CONSTANT_CHECK(int_<1> == integral_constant<int, 1>);
BOOST_HANA_CONSTANT_CHECK(int_<1> + long_<2> == long_<3>);
BOOST_HANA_CONSTANT_CHECK(!(bool_<true> && bool_<false>));
//! [integral_operators]

int main() { }
