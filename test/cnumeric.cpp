/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <test/cnumeric.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/injection.hpp>

// tested instances
#include <test/auto/constant.hpp>
#include <test/auto/logical.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <typename T>
    auto instances<CNumeric<T>> = tuple(
        type<Constant>,
        type<Logical>
    );

    template <typename T>
    auto objects<CNumeric<T>> = tuple(
        cnumeric<T, 0>,
        cnumeric<T, 1>,
        cnumeric<T, 2>,
        cnumeric<T, 3>
    );
}}}

namespace boost { namespace hana {
    template <typename T>
    struct operators::of<test::CNumeric<T>>
        : operators::of<Logical>
    { };
}}


int main() {
    test::check_datatype<test::CNumeric<int>>();

    // Constant
    {
        // value
        {
            static_assert(value(test::cnumeric<int, 0>) == 0, "");
            static_assert(value(test::cnumeric<int, 1>) == 1, "");
        }
    }

    // Logical
    {
        struct invalid { };
        constexpr auto true_ = test::cnumeric<bool, true>;
        constexpr auto false_ = test::cnumeric<bool, false>;
        constexpr auto x = test::cnumeric<int, 1>;
        constexpr auto y = test::cnumeric<int, 2>;

        // not_
        {
            BOOST_HANA_CONSTANT_CHECK(equal(not_(true_), false_));
        }

        // and_
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                and_(true_),
                true_
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                and_(false_),
                false_
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                and_(true_, true_),
                true_
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                and_(true_, false_),
                false_
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                and_(false_, invalid{}),
                false_
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                and_(true_, true_, true_),
                true_
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                and_(true_, true_, false_),
                false_
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                and_(true_, false_, invalid{}),
                false_
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                and_(false_, invalid{}, invalid{}),
                false_
            ));
        }

        // or_
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                or_(true_),
                true_
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                or_(false_),
                false_
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                or_(false_, false_),
                false_
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                or_(false_, true_),
                true_
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                or_(true_, invalid{}),
                true_
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                or_(false_, false_, false_),
                false_
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                or_(false_, false_, true_),
                true_
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                or_(false_, true_, invalid{}),
                true_
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                or_(true_, invalid{}, invalid{}),
                true_
            ));
        }

        // if_
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                if_(true_, x, y), x
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                if_(false_, x, y), y
            ));
        }

        // eval_if
        {
            auto t = [=](auto) { return x; };
            auto e = [=](auto) { return y; };

            BOOST_HANA_CONSTANT_CHECK(equal(
                eval_if(true_, t, invalid{}), x
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                eval_if(false_, invalid{}, e), y
            ));
        }

        // while_
        {
            //! @todo implement this
        }

        // until
        {
            using test::x;
            auto equal_to = curry<2>(equal);

            struct { } invalid{};
            auto f = test::injection([]{});

            BOOST_HANA_CONSTANT_CHECK(equal(
                until(equal_to(x<0>), x<0>, invalid),
                x<0>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                until(equal_to(f(x<0>)), x<0>, f),
                f(x<0>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                until(equal_to(f(f(x<0>))), x<0>, f),
                f(f(x<0>))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                until(equal_to(f(f(f(x<0>)))), x<0>, f),
                f(f(f(x<0>)))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                until(equal_to(f(f(f(f(x<0>))))), x<0>, f),
                f(f(f(f(x<0>))))
            ));

            // Make sure it can be called with an lvalue state:
            auto state = x<0>;
            BOOST_HANA_CONSTANT_CHECK(equal(
                until(equal_to(f(f(f(f(x<0>))))), state, f),
                f(f(f(f(x<0>))))
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;
            BOOST_HANA_CONSTANT_CHECK(equal(
                !true_,
                not_(true_)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                true_ && true_,
                and_(true_, true_)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                false_ || false_,
                or_(false_, false_)
            ));
        }
    }
}
