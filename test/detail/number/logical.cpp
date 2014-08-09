/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/logical/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/detail/number/logical.hpp>
using namespace boost::hana;


struct invalid {
    template <typename T>
    operator T const() { throw; }
};

template <typename Mcd>
void test() {
    constexpr auto logical = detail::number<Mcd>;
    constexpr auto comparable = detail::number<Mcd>;

    // not_
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            not_(logical(true)),
            logical(false)
        ));
    }

    // and_
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            and_(logical(true)),
            logical(true)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            and_(logical(false)),
            logical(false)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            and_(logical(true), logical(true)),
            logical(true)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            and_(logical(true), logical(false)),
            logical(false)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            and_(logical(false), invalid{}),
            logical(false)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            and_(logical(true), logical(true), logical(true)),
            logical(true)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            and_(logical(true), logical(true), logical(false)),
            logical(false)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            and_(logical(true), logical(false), invalid{}),
            logical(false)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            and_(logical(false), invalid{}, invalid{}),
            logical(false)
        ));
    }

    // or_
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            or_(logical(true)),
            logical(true)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            or_(logical(false)),
            logical(false)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            or_(logical(false), logical(false)),
            logical(false)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            or_(logical(false), logical(true)),
            logical(true)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            or_(logical(true), invalid{}),
            logical(true)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            or_(logical(false), logical(false), logical(false)),
            logical(false)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            or_(logical(false), logical(false), logical(true)),
            logical(true)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            or_(logical(false), logical(true), invalid{}),
            logical(true)
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            or_(logical(true), invalid{}, invalid{}),
            logical(true)
        ));
    }

    // if_
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            if_(logical(true), comparable(0), comparable(1)),
            comparable(0)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            if_(logical(false), comparable(0), comparable(1)),
            comparable(1)
        ));
    }

    // eval_if
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto t = [=](auto) { return comparable(0); };
        BOOST_HANA_CONSTEXPR_LAMBDA auto e = [=](auto) { return comparable(1); };

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            eval_if(logical(true), t, e),
            comparable(0)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            eval_if(logical(false), t, e),
            comparable(1)
        ));
    }

    // operators
    {
        constexpr auto logical = detail::number<Mcd, operators<Logical>>;
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            !logical(true),
            not_(logical(true))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            logical(true) && logical(true),
            and_(logical(true), logical(true))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            logical(false) || logical(false),
            or_(logical(false), logical(false))
        ));
    }
}

int main() {
    test<Logical::mcd>();
}
