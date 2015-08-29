/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/integral_constant.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/constant.hpp>
#include <laws/enumerable.hpp>
#include <laws/group.hpp>
#include <laws/integral_domain.hpp>
#include <laws/logical.hpp>
#include <laws/monoid.hpp>
#include <laws/orderable.hpp>
#include <laws/ring.hpp>

#include <cstddef>
#include <type_traits>
using namespace boost::hana;


void function() { }
void function_index(...) { }

int main() {
    using boost::hana::size_t; // take priority over ::size_t

    auto ints = make<Tuple>(
        int_<-10>, int_<-2>, int_<0>, int_<1>, int_<3>, int_<4>
    );
    (void)ints;

    auto bools = make<Tuple>(true_, false_);
    (void)bools;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // integral_constant's API (like std::integral_constant)
    //////////////////////////////////////////////////////////////////////////
    {
        // operator()
        static_assert(size_t<0>() == 0, "");
        static_assert(size_t<1>() == 1, "");
        static_assert(int_<-3>() == -3, "");

        // decltype(operator())
        static_assert(std::is_same<decltype(size_t<0>()), std::size_t>{}, "");
        static_assert(std::is_same<decltype(int_<-3>()), int>{}, "");

        // conversions
        constexpr std::size_t a = size_t<0>, b = size_t<1>;
        static_assert(a == 0 && b == 1, "");

        constexpr int c = int_<0>, d = int_<-3>;
        static_assert(c == 0 && d == -3, "");

        // nested ::value
        static_assert(decltype(int_<1>)::value == 1, "");

        // nested ::type
        static_assert(std::is_same<
            decltype(int_<1>)::type,
            std::remove_cv_t<decltype(int_<1>)>
        >{}, "");

        // nested ::value_type
        static_assert(std::is_same<decltype(int_<1>)::value_type, int>{}, "");
    }

    //////////////////////////////////////////////////////////////////////////
    // Make sure we can inherit integral_constant and retain the same
    // data type.
    //////////////////////////////////////////////////////////////////////////
    {
        struct derived : integral_constant<int, 10> { };
        static_assert(std::is_same<datatype_t<derived>, IntegralConstant<int>>{}, "");
    }

    //////////////////////////////////////////////////////////////////////////
    // Extensions to std::integral_constant
    //////////////////////////////////////////////////////////////////////////
    {
        // times member function
        {
            int counter = 0;
            int_<3>.times([&] { ++counter; });
            BOOST_HANA_RUNTIME_CHECK(counter == 3);

            // Call .times with a normal function used to fail.
            int_<3>.times(function);

            // make sure times can be accessed as a static member function too
            decltype(int_<5>)::times([]{ });

            // make sure xxx.times can be used as a function object
            auto z = int_<5>.times; (void)z;
        }

        // times.with_index
        {
            int index = 0;
            int_<3>.times.with_index([&](auto i) {
                static_assert(is_an<IntegralConstant<int>>(i), "");
                BOOST_HANA_RUNTIME_CHECK(value(i) == index);
                ++index;
            });

            // Calling .times.with_index with a normal function used to fail.
            int_<3>.times.with_index(function_index);

            // make sure times.with_index can be accessed as a static member
            // function too
            auto times = int_<5>.times;
            decltype(times)::with_index([](auto) { });

            // make sure xxx.times.with_index can be used as a function object
            auto z = int_<5>.times.with_index; (void)z;

            // make sure we're calling the function in the right order
            int current = 0;
            int_<5>.times.with_index([&](auto i) {
                BOOST_HANA_RUNTIME_CHECK(i == current);
                ++current;
            });
        }

        // Arithmetic operators
        {
            BOOST_HANA_CONSTANT_CHECK(+int_<1> == int_<1>);
            BOOST_HANA_CONSTANT_CHECK(-int_<1> == int_<-1>);
            BOOST_HANA_CONSTANT_CHECK(int_<1> + int_<2> == int_<3>);
            BOOST_HANA_CONSTANT_CHECK(int_<1> - int_<2> == int_<-1>);
            BOOST_HANA_CONSTANT_CHECK(int_<3> * int_<2> == int_<6>);
            BOOST_HANA_CONSTANT_CHECK(int_<6> / int_<3> == int_<2>);
            BOOST_HANA_CONSTANT_CHECK(int_<6> % int_<4> == int_<2>);
            BOOST_HANA_CONSTANT_CHECK(~int_<6> == int_<~6>);
            BOOST_HANA_CONSTANT_CHECK((int_<6> & int_<3>) == int_<6 & 3>);
            BOOST_HANA_CONSTANT_CHECK((int_<4> | int_<2>) == int_<4 | 2>);
            BOOST_HANA_CONSTANT_CHECK((int_<6> ^ int_<3>) == int_<6 ^ 3>);
            BOOST_HANA_CONSTANT_CHECK((int_<6> << int_<3>) == int_<(6 << 3)>);
            BOOST_HANA_CONSTANT_CHECK((int_<6> >> int_<3>) == int_<(6 >> 3)>);
        }

        // Comparison operators
        {
            BOOST_HANA_CONSTANT_CHECK(int_<0> == int_<0>);
            BOOST_HANA_CONSTANT_CHECK(int_<1> != int_<0>);
            BOOST_HANA_CONSTANT_CHECK(int_<0> < int_<1>);
            BOOST_HANA_CONSTANT_CHECK(int_<0> <= int_<1>);
            BOOST_HANA_CONSTANT_CHECK(int_<0> <= int_<0>);
            BOOST_HANA_CONSTANT_CHECK(int_<1> > int_<0>);
            BOOST_HANA_CONSTANT_CHECK(int_<1> >= int_<0>);
            BOOST_HANA_CONSTANT_CHECK(int_<0> >= int_<0>);
        }

        // Logical operators
        {
            BOOST_HANA_CONSTANT_CHECK(int_<3> || int_<0>);
            BOOST_HANA_CONSTANT_CHECK(int_<3> && int_<1>);
            BOOST_HANA_CONSTANT_CHECK(!int_<0>);
            BOOST_HANA_CONSTANT_CHECK(!!int_<3>);
        }

        // Creation with user-defined literals
        {
            using namespace boost::hana::literals;

            BOOST_HANA_CONSTANT_CHECK(0_c == llong<0>);
            BOOST_HANA_CONSTANT_CHECK(1_c == llong<1>);
            BOOST_HANA_CONSTANT_CHECK(12_c == llong<12>);
            BOOST_HANA_CONSTANT_CHECK(123_c == llong<123>);
            BOOST_HANA_CONSTANT_CHECK(1234567_c == llong<1234567>);
            BOOST_HANA_CONSTANT_CHECK(-34_c == llong<-34>);

            static_assert(std::is_same<
                decltype(-1234_c)::value_type,
                long long
            >{}, "");
            static_assert(-1234_c == -1234ll, "");
            BOOST_HANA_CONSTANT_CHECK(-12_c < 0_c);
        }
    }

#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    {
        // value
        static_assert(value(integral_c<int, 0>) == 0, "");
        static_assert(value(integral_c<int, 1>) == 1, "");

        // laws
        test::TestConstant<IntegralConstant<int>>{ints, tuple_t<int, long, long long>};
        test::TestConstant<IntegralConstant<bool>>{bools, tuple_t<bool>};
    }

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // Enumerable, Monoid, Group
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestEnumerable<IntegralConstant<int>>{ints};
        test::TestMonoid<IntegralConstant<int>>{ints};
        test::TestGroup<IntegralConstant<int>>{ints};
    }

#elif BOOST_HANA_TEST_PART == 4
    //////////////////////////////////////////////////////////////////////////
    // Ring, IntegralDomain
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestRing<IntegralConstant<int>>{ints};
        test::TestIntegralDomain<IntegralConstant<int>>{ints};
    }

#elif BOOST_HANA_TEST_PART == 5
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

        // laws
        auto ints = make<Tuple>(int_<-2>, int_<0>, int_<1>, int_<3>);
        test::TestLogical<IntegralConstant<int>>{ints};
        test::TestLogical<IntegralConstant<bool>>{bools};
    }

#elif BOOST_HANA_TEST_PART == 6
    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestComparable<IntegralConstant<int>>{ints};
    }

#elif BOOST_HANA_TEST_PART == 7
    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestOrderable<IntegralConstant<int>>{ints};
    }
#endif
}
