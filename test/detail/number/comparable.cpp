/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/comparable/not_equal_mcd.hpp>

#include <boost/hana/comparable/detail/laws.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/logical/logical.hpp>
using namespace boost::hana;


template <typename Mcd>
void test() {
    constexpr auto comparable = detail::number<Mcd>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});

    // equal
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(comparable(0), comparable(0)));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(comparable(0), comparable(1))));
    }

    // not_equal
    {
        BOOST_HANA_CONSTEXPR_ASSERT(not_equal(comparable(0), comparable(1)));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(not_equal(comparable(0), comparable(0))));
    }

    // comparing
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            comparing(f)(comparable(0), comparable(0)),
            equal(f(comparable(0)), f(comparable(0)))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            comparing(f)(comparable(0), comparable(1)),
            equal(f(comparable(0)), f(comparable(1)))
        ));
    }

    // operators
    {
        constexpr auto comparable = detail::number<Mcd, operators<Comparable>>;
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            comparable(0) == comparable(0),
            equal(comparable(0), comparable(0))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            comparable(0) != comparable(0),
            not_equal(comparable(0), comparable(0))
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Comparable::laws::check(
            list(comparable(0), comparable(1), comparable(2))
        ));
    }
}

int main() {
    test<Comparable::equal_mcd>();
    test<Comparable::not_equal_mcd>();
}
