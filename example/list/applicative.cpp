/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/pair.hpp>

#include <tuple>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(lift<List>('x') == list('x'));
    BOOST_HANA_STATIC_ASSERT(lift<StdTuple>('x') == std::make_tuple('x'));

    BOOST_HANA_CONSTEXPR_LAMBDA auto f = pair;
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = flip(pair);
    BOOST_HANA_STATIC_ASSERT(
        ap(list(f, g), list(1, 2, 3), list('a', 'b'))
            ==
        list(
            f(1, 'a'), f(1, 'b'), f(2, 'a'), f(2, 'b'), f(3, 'a'), f(3, 'b'),
            g(1, 'a'), g(1, 'b'), g(2, 'a'), g(2, 'b'), g(3, 'a'), g(3, 'b')
        )
    );
    //! [main]
}
