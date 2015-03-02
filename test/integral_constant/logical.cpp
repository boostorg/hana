/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral_constant.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/logical.hpp>
using namespace boost::hana;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // Logical
    //////////////////////////////////////////////////////////////////////////
    {
        auto t = test::ct_eq<3>{};
        auto e = test::ct_eq<4>{};

        // eval_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                eval_if(true_, always(t), always(e)),
                t
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                eval_if(false_, always(t), always(e)),
                e
            ));
        }

        // not_
        {
            BOOST_HANA_CONSTANT_CHECK(equal(not_(true_), false_));
            BOOST_HANA_CONSTANT_CHECK(equal(not_(false_), true_));
        }

        // operators
        static_assert(has_operator<IntegralConstant<int>, decltype(not_)>{}, "");
        static_assert(has_operator<IntegralConstant<int>, decltype(and_)>{}, "");
        static_assert(has_operator<IntegralConstant<int>, decltype(or_)>{}, "");

        // laws
        test::TestLogical<IntegralConstant<int>>{tuple_c<int, -2, 0, 1, 2>};
        test::TestLogical<IntegralConstant<bool>>{tuple_c<bool, true, false>};
    }
}
