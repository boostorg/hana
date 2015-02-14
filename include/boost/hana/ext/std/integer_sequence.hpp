/*!
@file
Defines `boost::hana::ext::std::IntegerSequence`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_HPP
#define BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_HPP

#include <boost/hana/fwd/ext/std/integer_sequence.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>

// instances
#include <boost/hana/foldable.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/searchable.hpp>

#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<ext::std::IntegerSequence, ext::std::IntegerSequence> {
        template <typename X, X ...xs, typename Y, Y ...ys>
        static constexpr auto apply(
            ::std::integer_sequence<X, xs...>,
            ::std::integer_sequence<Y, ys...>,
            // this dummy parameter disables this specialization if
            // sizeof...(xs) != sizeof...(ys)
            char(*)[sizeof...(xs) == sizeof...(ys)] = 0)
        {
            return bool_<detail::std::is_same<
                ::std::integer_sequence<bool, (xs == ys)...>,
                ::std::integer_sequence<bool, (xs, true)...>
            >::value>;
        }

        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs, Ys, ...)
        { return false_; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct foldl_impl<ext::std::IntegerSequence>
        : Iterable::foldl_impl<ext::std::IntegerSequence>
    { };

    template <>
    struct foldr_impl<ext::std::IntegerSequence>
        : Iterable::foldr_impl<ext::std::IntegerSequence>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_impl<ext::std::IntegerSequence>
        : Iterable::find_impl<ext::std::IntegerSequence>
    { };

    template <>
    struct any_impl<ext::std::IntegerSequence>
        : Iterable::any_impl<ext::std::IntegerSequence>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct head_impl<ext::std::IntegerSequence> {
        template <typename T, T x, T ...xs>
        static constexpr auto apply(::std::integer_sequence<T, x, xs...>)
        { return ::std::integral_constant<T, x>{}; }
    };

    template <>
    struct tail_impl<ext::std::IntegerSequence> {
        template <typename T, T x, T ...xs>
        static constexpr auto apply(::std::integer_sequence<T, x, xs...>)
        { return ::std::integer_sequence<T, xs...>{}; }
    };

    template <>
    struct is_empty_impl<ext::std::IntegerSequence> {
        template <typename T, T ...xs>
        static constexpr auto apply(::std::integer_sequence<T, xs...>)
        { return bool_<sizeof...(xs) == 0>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_HPP
