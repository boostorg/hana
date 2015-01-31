/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral_constant.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <cstddef>
#include <test/auto/base.hpp>
#include <test/auto/constant.hpp>
#include <test/auto/integral_constant.hpp>
#include <type_traits>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <typename T>
    auto objects<Integral<T>> = tuple(
        integral_constant<T, 0>,
        integral_constant<T, 1>,
        integral_constant<T, 2>,
        integral_constant<T, 3>
    );

    template <typename T>
    auto instances<Integral<T>> = tuple(
        type<Constant>,
        type<IntegralConstant>
    );
}}}


int main() {
    test::check_datatype<Integral<int>>();

    // Integral's API (like std::integral_constant)
    {
        // operator()
        static_assert(size_t<0>() == 0, "");
        static_assert(size_t<1>() == 1, "");
        static_assert(int_<-3>() == -3, "");

        // decltype(operator())
        BOOST_HANA_CONSTANT_CHECK(decltype_(size_t<0>()) == type<std::size_t>);
        BOOST_HANA_CONSTANT_CHECK(decltype_(int_<-3>()) == type<int>);

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

    // Extensions to std::integral_constant
    {
        // times member function
        {
            int counter = 0;
            int_<3>.times([&] { ++counter; });
            BOOST_HANA_RUNTIME_CHECK(counter == 3);
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
            BOOST_HANA_CONSTANT_CHECK(int_<6> | int_<3> == int_<6 | 3>);
            BOOST_HANA_CONSTANT_CHECK(int_<6> ^ int_<3> == int_<6 ^ 3>);
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

            BOOST_HANA_CONSTANT_CHECK(decltype_(-1234_c) == decltype_(llong<-1234>));
            BOOST_HANA_CONSTANT_CHECK(-12_c < 0_c);
        }
    }

    // Constant
    {
        // value
        {
            static_assert(value(integral<int, 0>) == 0, "");
            static_assert(value(integral<int, 1>) == 1, "");
        }
    }

    // IntegralConstant
    {
        // integral_constant
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                integral_constant<Integral<int>, 3>(),
                integral<int, 3>
            ));
        }
    }
}
