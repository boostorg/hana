/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool/comparable.hpp>
#include <boost/hana/bool/logical.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/injection.hpp>
using namespace boost::hana;


int main() {
    using test::x;
    BOOST_HANA_CONSTEXPR_LAMBDA auto t = [=](auto) { return x<0>; };
    BOOST_HANA_CONSTEXPR_LAMBDA auto e = [=](auto) { return x<1>; };

    // eval_if
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            eval_if(true_, t, e), x<0>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            eval_if(false_, t, e), x<1>
        ));
    }

    // not_
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(not_(true_), false_));
        BOOST_HANA_CONSTANT_ASSERT(equal(not_(false_), true_));
    }
}
