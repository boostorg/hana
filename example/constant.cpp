/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {

{

//! [value]
auto i = integral_constant<int, 3>; // notice no constexpr
static_assert(value<decltype(i)>() == 3, "");
static_assert(value(i) == 3, "value(i) is always a constant expression!");
//! [value]

}{

//! [value_of]
constexpr auto xs = tuple_c<int, 1, 2, 3, 4, 5>;
constexpr auto vs = transform(xs, value_of);
static_assert(vs == make<Tuple>(1, 2, 3, 4, 5), "");
//! [value_of]

}

}

