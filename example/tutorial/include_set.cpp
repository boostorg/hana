/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>

//! [main]
#include <boost/hana/set.hpp>
using namespace boost::hana;


int main() {
  constexpr auto xs = set(1, 2.2, 'x');
  static_assert(contains(xs, 'x'), "");
}
//! [main]
