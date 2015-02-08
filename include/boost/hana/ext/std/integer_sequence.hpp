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
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>

// instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/iterable.hpp>

#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Comparable(ext::std::IntegerSequence)>
        : detail::std::true_type
    { };

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

    template <>
    struct Iterable::instance<ext::std::IntegerSequence> : Iterable::mcd {
        template <typename T, T x, T ...xs>
        static constexpr auto head_impl(::std::integer_sequence<T, x, xs...>)
        { return ::std::integral_constant<T, x>{}; }

        template <typename T, T x, T ...xs>
        static constexpr auto tail_impl(::std::integer_sequence<T, x, xs...>)
        { return ::std::integer_sequence<T, xs...>{}; }

        template <typename T, T ...xs>
        static constexpr auto is_empty_impl(::std::integer_sequence<T, xs...>)
        { return bool_<sizeof...(xs) == 0>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_INTEGER_SEQUENCE_HPP
