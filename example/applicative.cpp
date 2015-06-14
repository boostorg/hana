/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/functional/placeholder.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {

{

//! [ap]
static_assert(
    ap(make_tuple(_ + _), make_tuple(1, 2), make_tuple(3, 4, 5))
        ==
    make_tuple(
        1 + 3,      1 + 4,      1 + 5,
        2 + 3,      2 + 4,      2 + 5
    )
, "");

BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto a, auto b, auto c) {
    return a * b * c;
};
BOOST_HANA_CONSTEXPR_CHECK(
    ap(just(g), just(1), just(2), just(3)) == just(1 * 2 * 3)
);
BOOST_HANA_CONSTANT_CHECK(
    ap(just(g), just(1), nothing, just(3)) == nothing
);
//! [ap]

}{

//! [lift]
static_assert(lift<Tuple>('x') == make_tuple('x'), "");
static_assert(lift<Optional>('x') == just('x'), "");
//! [lift]

}

}
