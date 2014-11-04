/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    {
        //! [sequence]
        BOOST_HANA_CONSTEXPR_ASSERT(
            sequence<Maybe>(tuple(just(1), just('2'), just(3.3))) == just(tuple(1, '2', 3.3))
        );

        BOOST_HANA_CONSTANT_ASSERT(
            sequence<Maybe>(tuple(just(1), nothing, just(3.3))) == nothing
        );

        // This is a generalized Cartesian product.
        BOOST_HANA_CONSTEXPR_ASSERT(
            sequence<Tuple>(tuple(tuple(1, 2, 3), tuple(4), tuple(5, 6)))
            ==
            tuple(
                tuple(1, 4, 5), tuple(1, 4, 6),
                tuple(2, 4, 5), tuple(2, 4, 6),
                tuple(3, 4, 5), tuple(3, 4, 6)
            )
        );
        //! [sequence]
    }

    {
        //! [traverse]
        BOOST_HANA_CONSTEXPR_LAMBDA auto half = [](auto x) {
            return if_(x % int_<2> == int_<0>,
                just(x / int_<2>),
                nothing
            );
        };

        BOOST_HANA_CONSTANT_ASSERT(
            traverse<Maybe>(tuple(int_<2>, int_<4>, int_<6>), half)
            ==
            just(tuple(int_<1>, int_<2>, int_<3>))
        );

        BOOST_HANA_CONSTANT_ASSERT(
            traverse<Maybe>(tuple(int_<2>, int_<3>, int_<6>), half)
            ==
            nothing
        );

        BOOST_HANA_CONSTEXPR_LAMBDA auto twice = [](auto x) {
            return tuple(x, x);
        };

        BOOST_HANA_CONSTEXPR_ASSERT(
            traverse<Tuple>(just('x'), twice) == tuple(just('x'), just('x'))
        );

        BOOST_HANA_CONSTANT_ASSERT(
            traverse<Tuple>(nothing, twice) == tuple(nothing)
        );
        //! [traverse]
    }
}
