/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foreign.hpp> //! @todo Find a way to remove this
#include <boost/hana/fwd/comparable.hpp>

#include <test/injection.hpp>
#include <test/laws/comparable.hpp>
#include <test/numeric/comparable.hpp>
using namespace boost::hana;


namespace boost { namespace hana {
    template <> struct enabled_operators<test::Numeric> : Comparable { };
}}

int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});
    BOOST_HANA_CONSTEXPR_LAMBDA auto x = test::numeric(1);
    BOOST_HANA_CONSTEXPR_LAMBDA auto y = test::numeric(2);
    BOOST_HANA_CONSTEXPR_LAMBDA auto z = test::numeric(3);

    // equal
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(x, x));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(x, y)));
    }

    // not_equal
    {
        BOOST_HANA_CONSTEXPR_ASSERT(not_equal(x, y));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(not_equal(x, x)));
    }

    // comparing
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            comparing(f)(x, x),
            equal(f(x), f(x))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            comparing(f)(x, y),
            equal(f(x), f(y))
        ));
    }

    // operators
    {
        using namespace boost::hana::operators;
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            x == y,
            equal(x, y)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            x != y,
            not_equal(x, y)
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Comparable_laws(
            x, y, z
        ));
    }
}
