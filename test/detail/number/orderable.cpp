/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/orderable/less_mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/detail/number/orderable.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/orderable/detail/laws.hpp>
using namespace boost::hana;


template <typename Mcd>
void test() {
    constexpr auto ord = detail::number<Mcd>;
    // detail::injection is monotonic too
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});

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

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Orderable::laws::check(
            list(ord(0), ord(1), ord(2))
        ));
    }

    // operators
    {
        constexpr auto ord = detail::number<Mcd, operators<Orderable>>;
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
}

int main() {
    test<Orderable::less_mcd>();
}
