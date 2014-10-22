/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>

#include <tuple>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_ASSERT(lift<Tuple>('x') == tuple('x'));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(lift<ext::std::Tuple>('x'), std::make_tuple('x')));

    BOOST_HANA_CONSTEXPR_LAMBDA auto f = pair;
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = flip(pair);
    BOOST_HANA_CONSTEXPR_ASSERT(
        ap(tuple(f, g), tuple(1, 2, 3), tuple('a', 'b'))
            ==
        tuple(
            f(1, 'a'), f(1, 'b'), f(2, 'a'), f(2, 'b'), f(3, 'a'), f(3, 'b'),
            g(1, 'a'), g(1, 'b'), g(2, 'a'), g(2, 'b'), g(3, 'a'), g(3, 'b')
        )
    );
    //! [main]
}
