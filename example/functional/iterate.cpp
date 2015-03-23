/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional/iterate.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
//! [main]
constexpr auto next_10 = iterate<10>(succ);
static_assert(next_10(3) == 13, "");

constexpr auto xs = make<Tuple>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
static_assert(iterate<5>(tail, xs) == make<Tuple>(6, 7, 8, 9, 10), "");
//! [main]
}
