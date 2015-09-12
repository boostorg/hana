/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <test/cnumeric.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/constant.hpp>
#include <laws/group.hpp>
#include <laws/integral_domain.hpp>
#include <laws/logical.hpp>
#include <laws/monoid.hpp>
#include <laws/orderable.hpp>
#include <laws/ring.hpp>
using namespace boost::hana;


struct invalid { };
using test::ct_eq;

int main() {
    auto ints = make<tuple_tag>(
        test::cnumeric<int, -3>,
        test::cnumeric<int, 0>,
        test::cnumeric<int, 1>,
        test::cnumeric<int, 2>,
        test::cnumeric<int, 3>
    );
    (void)ints;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    {
        // value
        static_assert(value(test::cnumeric<int, 0>) == 0, "");
        static_assert(value(test::cnumeric<int, 1>) == 1, "");

        // laws
        test::TestConstant<test::CNumeric<int>>{ints, tuple_t<int, long, long long>};
    }

    //////////////////////////////////////////////////////////////////////////
    // Monoid, Group, Ring, IntegralDomain
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestMonoid<test::CNumeric<int>>{ints};
        test::TestGroup<test::CNumeric<int>>{ints};
        test::TestRing<test::CNumeric<int>>{ints};
        test::TestIntegralDomain<test::CNumeric<int>>{ints};
    }

#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    test::TestComparable<test::CNumeric<int>>{ints};

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    test::TestOrderable<test::CNumeric<int>>{ints};

#elif BOOST_HANA_TEST_PART == 4
    //////////////////////////////////////////////////////////////////////////
    // Logical
    //////////////////////////////////////////////////////////////////////////
    {
        constexpr auto true_ = test::cnumeric<bool, true>;
        constexpr auto false_ = test::cnumeric<bool, false>;

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
                if_(true_, ct_eq<3>{}, ct_eq<4>{}), ct_eq<3>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                if_(false_, ct_eq<3>{}, ct_eq<4>{}), ct_eq<4>{}
            ));
        }

        // eval_if
        {
            auto t = [](auto) { return ct_eq<2>{}; };
            auto e = [](auto) { return ct_eq<3>{}; };

            BOOST_HANA_CONSTANT_CHECK(equal(
                eval_if(true_, t, invalid{}), ct_eq<2>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                eval_if(false_, invalid{}, e), ct_eq<3>{}
            ));
        }

        // while_
        {
            test::_injection<0> f{};

            BOOST_HANA_CONSTANT_CHECK(equal(
                while_(not_equal.to(ct_eq<0>{}), ct_eq<0>{}, invalid{}),
                ct_eq<0>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                while_(not_equal.to(f(ct_eq<0>{})), ct_eq<0>{}, f),
                f(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                while_(not_equal.to(f(f(ct_eq<0>{}))), ct_eq<0>{}, f),
                f(f(ct_eq<0>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                while_(not_equal.to(f(f(f(ct_eq<0>{})))), ct_eq<0>{}, f),
                f(f(f(ct_eq<0>{})))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                while_(not_equal.to(f(f(f(f(ct_eq<0>{}))))), ct_eq<0>{}, f),
                f(f(f(f(ct_eq<0>{}))))
            ));

            // Make sure it can be called with an lvalue state:
            auto state = ct_eq<0>{};
            BOOST_HANA_CONSTANT_CHECK(equal(
                while_(not_equal.to(f(f(f(f(ct_eq<0>{}))))), state, f),
                f(f(f(f(ct_eq<0>{}))))
            ));
        }

        // laws
        test::TestLogical<test::CNumeric<bool>>{make<tuple_tag>(
            test::cnumeric<bool, true>, test::cnumeric<bool, false>
        )};
    }
#endif
}
