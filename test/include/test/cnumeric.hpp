/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_CNUMERIC_HPP
#define BOOST_HANA_TEST_TEST_CNUMERIC_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_integral.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/orderable.hpp>


namespace boost { namespace hana {
    namespace test {
        template <typename T>
        struct CNumeric { using value_type = T; };

        template <typename T, T v>
        struct cnumeric_type {
            static constexpr T value = v;
            struct hana { using datatype = CNumeric<T>; };

            //! @todo Find a way to make this explicit.
            constexpr operator bool() const { return value; }
        };

        template <typename T, T v>
        constexpr cnumeric_type<T, v> cnumeric{};
    }

    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models<Constant(test::CNumeric<T>)>
        : detail::std::true_type
    { };

    template <typename T>
    struct value_impl<test::CNumeric<T>> {
        template <typename X>
        static constexpr auto apply(X const&)
        { return X::value; }
    };

    template <typename T, typename C>
    struct to_impl<test::CNumeric<T>, C, when<
        models<Constant(C)>{} &&
        detail::std::is_integral<typename C::value_type>{}
    >>
        : embedding<is_embedded<typename C::value_type, T>{}>
    {
        static_assert(detail::std::is_integral<T>{},
        "trying to convert a Constant to a test::CNumeric of a non-integral "
        "type; test::CNumeric may only hold integral types");

        template <typename X>
        static constexpr auto apply(X x) {
            constexpr auto v = hana::value(x);
            return test::cnumeric<T, static_cast<T>(v)>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models<Orderable(test::CNumeric<T>)>
        : detail::std::true_type
    { };

    template <typename T, typename U>
    struct less_impl<test::CNumeric<T>, test::CNumeric<U>> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y)
        { return test::cnumeric<bool, (X::value < Y::value)>; }
    };

    // Define either one to select which MCD is used:
    //  BOOST_HANA_TEST_COMPARABLE_EQUAL_MCD
    //  BOOST_HANA_TEST_COMPARABLE_NOT_EQUAL_MCD
    //  BOOST_HANA_TEST_COMPARABLE_ORDERABLE_MCD
    //
    // If neither is defined, the MCD used is unspecified.
#ifdef BOOST_HANA_TEST_COMPARABLE_EQUAL_MCD
    template <typename T, typename U>
    struct Comparable::instance<test::CNumeric<T>, test::CNumeric<U>>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
        { return test::cnumeric<bool, X::value == Y::value>; }
    };
#elif defined(BOOST_HANA_TEST_COMPARABLE_NOT_EQUAL_MCD)
    template <typename T, typename U>
    struct Comparable::instance<test::CNumeric<T>, test::CNumeric<U>>
        : Comparable::not_equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto not_equal_impl(X x, Y y)
        { return test::cnumeric<bool, X::value != Y::value>; }
    };
#else
    template <typename T, typename U>
    struct Comparable::instance<test::CNumeric<T>, test::CNumeric<U>>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y) {
            return Orderable::equal_impl<test::CNumeric<T>, test::CNumeric<U>>::apply(x, y);
        }
    };
#endif
}}

#endif // !BOOST_HANA_TEST_TEST_CNUMERIC_HPP
