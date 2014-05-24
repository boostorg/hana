/*!
 * @file
 * Defines `boost::hana::Range`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RANGE_HPP
#define BOOST_HANA_RANGE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/comparable_from_iterable.hpp>
#include <boost/hana/detail/foldable_from_iterable.hpp>
#include <boost/hana/detail/integer_sequence.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    //! @ingroup datatypes
    template <typename T, T ...v>
    struct Range { };

    constexpr struct _range {
        template <typename T, T from, T to>
        constexpr auto operator()(Integral<T, from>, Integral<T, to>) const {
            return typename detail::make_integer_sequence<T, to - from>::
                   template slide_by<from, Range>{};
        }
    } range{};

    template <typename T>
    struct Iterable<Range<T>> : defaults<Iterable> {
        static constexpr Bool<true> is_empty_impl(Range<T>)
        { return {}; }
    };

    template <typename T, T v, T ...vs>
    struct Iterable<Range<T, v, vs...>> : defaults<Iterable> {
        static constexpr Integral<T, v> head_impl(Range<T, v, vs...>)
        { return {}; }

        static constexpr Range<T, vs...> tail_impl(Range<T, v, vs...>)
        { return {}; }

        static constexpr Bool<false> is_empty_impl(Range<T, v, vs...>)
        { return {}; }
    };

    template <typename T, T ...vs>
    struct Foldable<Range<T, vs...>>
        : detail::foldable_from_iterable
    { };

    template <typename T, T ...ts, typename U, U ...us>
    struct Comparable<Range<T, ts...>, Range<U, us...>> : defaults<Comparable> {
        static constexpr Bool<false>
        equal_helper(Range<T, ts...>, Range<U, us...>, Bool<false>)
        { return {}; }

        template <typename X, X ...xs, typename Y, Y ...ys>
        static constexpr auto
        equal_helper(Range<X, xs...>, Range<Y, ys...>, Bool<true>)
        { return and_(bool_<xs == ys>...); }

        static constexpr auto equal_impl(Range<T, ts...> a, Range<U, us...> b)
        { return equal_helper(a, b, bool_<sizeof...(ts) == sizeof...(us)>); }
    };

    template <typename T, T ...ts, typename U, U ...us>
    constexpr auto operator==(Range<T, ts...> a, Range<U, us...> b)
    {  return equal(a, b); }

    template <typename T, T ...ts, typename U, U ...us>
    constexpr auto operator!=(Range<T, ts...> a, Range<U, us...> b)
    {  return not_equal(a, b); }
}} // end namespace boost::hana

#include <boost/hana/list.hpp>

namespace boost { namespace hana {
    template <typename T, T ...vs>
    struct Functor<Range<T, vs...>> : defaults<Functor> {
        template <typename F>
        static constexpr auto fmap_impl(F f, Range<T, vs...>)
        { return list(f(Integral<T, vs>{})...); }
    };
}}

#endif // !BOOST_HANA_RANGE_HPP
