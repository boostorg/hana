/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foreign/comparable.hpp>
#include <boost/hana/foreign/logical.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/numeric/comparable.hpp>
using namespace boost::hana;


struct foreign {
    bool value;
    constexpr operator bool() const { return value; }
};

int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto x = test::numeric(0);
    BOOST_HANA_CONSTEXPR_LAMBDA auto y = test::numeric(1);
    BOOST_HANA_CONSTEXPR_LAMBDA auto t = [=](auto) { return x; };
    BOOST_HANA_CONSTEXPR_LAMBDA auto e = [=](auto) { return y; };

    // eval_if
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            eval_if(foreign{true}, t, e), x
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            eval_if(foreign{false}, t, e), y
        ));
    }

    // not_
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(not_(foreign{true}), false));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(not_(foreign{false}), true));
    }
}
