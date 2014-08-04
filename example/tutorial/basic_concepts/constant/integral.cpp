/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include <type_traits>
using namespace boost::hana;


//! [integral_create]
auto one = integral<int, 1>;
auto yes = integral<bool, true>;
//! [integral_create]

//! [integral_api]
using One = decltype(one);

static_assert(std::is_same<One::type, One>{}, "");

static_assert(std::is_same<One::value_type, int>{}, "");

static_assert(One::value == 1               &&
              static_cast<int>(one) == 1    &&
              one() == 1                    &&
              value(one) == 1
, "these are all constant expressions");
//! [integral_api]

namespace anon {
//! [integral_shorthands]
auto one = int_<1>;
auto yes = bool_<true>;
//! [integral_shorthands]
}

int main() {
//! [integral_operators]
BOOST_HANA_STATIC_ASSERT(int_<1> == integral<int, 1>);
BOOST_HANA_STATIC_ASSERT(int_<1> + long_<2> == long_<3>);
BOOST_HANA_STATIC_ASSERT(!(bool_<true> && bool_<false>));
//! [integral_operators]
}
