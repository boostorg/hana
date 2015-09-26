/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_CNUMERIC_HPP
#define BOOST_HANA_TEST_TEST_CNUMERIC_HPP

#include <boost/hana/concept/integral_constant.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/fwd/core/convert.hpp>
#include <boost/hana/fwd/equal.hpp>
#include <boost/hana/fwd/less.hpp>


namespace boost { namespace hana {
    namespace test {
        template <typename T>
        struct CNumeric { using value_type = T; };

        template <typename T, T v>
        struct cnumeric_t {
            static constexpr T value = v;
            using hana_tag = CNumeric<T>;
            constexpr operator T() const { return value; }
        };

        template <typename T, T v>
        constexpr cnumeric_t<T, v> cnumeric{};

        template <typename T, T v>
        constexpr cnumeric_t<T, v> make_cnumeric() { return {}; }
    }

    //////////////////////////////////////////////////////////////////////////
    // Constant/IntegralConstant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct IntegralConstant<test::CNumeric<T>> {
        static constexpr bool value = true;
    };

    template <typename T, typename C>
    struct to_impl<test::CNumeric<T>, C, when<
        hana::IntegralConstant<C>::value
    >>
        : embedding<is_embedded<typename C::value_type, T>::value>
    {
        template <typename N>
        static constexpr auto apply(N const&) {
            return test::cnumeric<T, N::value>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U>
    struct equal_impl<test::CNumeric<T>, test::CNumeric<U>> {
        template <typename X, typename Y>
        static constexpr auto apply(X const&, Y const&)
        { return test::cnumeric<bool, X::value == Y::value>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U>
    struct less_impl<test::CNumeric<T>, test::CNumeric<U>> {
        template <typename X, typename Y>
        static constexpr auto apply(X const&, Y const&)
        { return test::cnumeric<bool, (X::value < Y::value)>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TEST_TEST_CNUMERIC_HPP
