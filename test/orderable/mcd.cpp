/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foreign.hpp> //! @todo Find a way to get rid of this
#include <boost/hana/logical.hpp>
#include <boost/hana/orderable.hpp>

#include <test/laws/orderable.hpp>
#include <test/injection.hpp>
#include <test/numeric/comparable.hpp>
#include <test/numeric/orderable.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto ord = [](auto x) {
        return test::numeric(x);
    };

    // test::injection is also monotonic
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});

    // less
    {
        BOOST_HANA_CONSTEXPR_ASSERT(less(ord(0), ord(1)));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(less(ord(0), ord(0))));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(less(ord(1), ord(0))));
    }

    // less_equal
    {
        BOOST_HANA_CONSTEXPR_ASSERT(less_equal(ord(0), ord(1)));
        BOOST_HANA_CONSTEXPR_ASSERT(less_equal(ord(0), ord(0)));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(less_equal(ord(1), ord(0))));
    }

    // greater_equal
    {
        BOOST_HANA_CONSTEXPR_ASSERT(greater_equal(ord(1), ord(0)));
        BOOST_HANA_CONSTEXPR_ASSERT(greater_equal(ord(0), ord(0)));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(greater_equal(ord(0), ord(1))));
    }

    // greater
    {
        BOOST_HANA_CONSTEXPR_ASSERT(greater(ord(1), ord(0)));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(greater(ord(0), ord(0))));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(greater(ord(0), ord(1))));
    }

    // max
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            max(ord(0), ord(0)), ord(0)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            max(ord(1), ord(0)), ord(1)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            max(ord(0), ord(1)), ord(1)
        ));
    }

    // min
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            min(ord(0), ord(0)),
            ord(0)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            min(ord(1), ord(0)),
            ord(0)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            min(ord(0), ord(1)),
            ord(0)
        ));
    }

    // ordering
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            ordering(f)(ord(1), ord(0)),
            less(f(ord(1)), f(ord(0)))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            ordering(f)(ord(0), ord(1)),
            less(f(ord(0)), f(ord(1)))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            ordering(f)(ord(0), ord(0)),
            less(f(ord(0)), f(ord(0)))
        ));
    }

    // operators
    {
        using namespace boost::hana::operators;
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            ord(0) < ord(1),
            less(ord(0), ord(1))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            ord(0) <= ord(1),
            less_equal(ord(0), ord(1))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            ord(0) > ord(1),
            greater(ord(0), ord(1))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            ord(0) >= ord(1),
            greater_equal(ord(0), ord(1))
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Orderable_laws(
            ord(0), ord(1), ord(2)
        ));
    }
}
