/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/auto/comparable.hpp>
#include <test/auto/constant.hpp>
#include <test/auto/logical.hpp>
#include <test/injection.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<Bool> = tuple(
        type<Comparable>,
        type<Logical>,
        type<Constant>
    );

    template <>
    auto objects<Bool> = tuple(true_, false_);
}}}


int main() {
    test::check_datatype<Bool>();

    // Comparable
    {
        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(true_, true_));
            BOOST_HANA_CONSTANT_CHECK(equal(false_, false_));

            BOOST_HANA_CONSTANT_CHECK(not_(equal(false_, true_)));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(true_, false_)));
        }
    }

    // Constant
    {
        // value
        {
            static_assert(value(true_) == true, "");
            static_assert(value(false_) == false, "");
        }
    }

    // Logical
    {
        using test::x;
        auto t = [=](auto) { return x<0>; };
        auto e = [=](auto) { return x<1>; };

        // eval_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                eval_if(true_, t, e), x<0>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                eval_if(false_, t, e), x<1>
            ));
        }

        // not_
        {
            BOOST_HANA_CONSTANT_CHECK(equal(not_(true_), false_));
            BOOST_HANA_CONSTANT_CHECK(equal(not_(false_), true_));
        }
    }
}
