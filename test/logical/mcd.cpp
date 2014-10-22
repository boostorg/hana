/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/logical.hpp>

#include <test/cnumeric/comparable.hpp>
#include <test/cnumeric/constant.hpp>
#include <test/cnumeric/logical.hpp>
using namespace boost::hana;


namespace boost { namespace hana {
    template <> struct enabled_operators<test::CNumeric> : Logical { };
}}

int main() {
    struct invalid { };
    constexpr auto true_ = test::cnumeric<bool, true>;
    constexpr auto false_ = test::cnumeric<bool, false>;
    constexpr auto x = test::cnumeric<int, 1>;
    constexpr auto y = test::cnumeric<int, 2>;

    // not_
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(not_(true_), false_));
    }

    // and_
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            and_(true_),
            true_
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            and_(false_),
            false_
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            and_(true_, true_),
            true_
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            and_(true_, false_),
            false_
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            and_(false_, invalid{}),
            false_
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            and_(true_, true_, true_),
            true_
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            and_(true_, true_, false_),
            false_
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            and_(true_, false_, invalid{}),
            false_
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            and_(false_, invalid{}, invalid{}),
            false_
        ));
    }

    // or_
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            or_(true_),
            true_
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            or_(false_),
            false_
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            or_(false_, false_),
            false_
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            or_(false_, true_),
            true_
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            or_(true_, invalid{}),
            true_
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            or_(false_, false_, false_),
            false_
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            or_(false_, false_, true_),
            true_
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            or_(false_, true_, invalid{}),
            true_
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            or_(true_, invalid{}, invalid{}),
            true_
        ));
    }

    // if_
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            if_(true_, x, y), x
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            if_(false_, x, y), y
        ));
    }

    // eval_if
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto t = [=](auto) { return x; };
        BOOST_HANA_CONSTEXPR_LAMBDA auto e = [=](auto) { return y; };

        BOOST_HANA_CONSTANT_ASSERT(equal(
            eval_if(true_, t, invalid{}), x
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            eval_if(false_, invalid{}, e), y
        ));
    }

    // operators
    {
        using namespace boost::hana::operators;
        BOOST_HANA_CONSTANT_ASSERT(equal(
            !true_,
            not_(true_)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            true_ && true_,
            and_(true_, true_)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            false_ || false_,
            or_(false_, false_)
        ));
    }
}
