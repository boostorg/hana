/*!
@file
Defines `boost::hana::Range`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RANGE_HPP
#define BOOST_HANA_RANGE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/comparable_from_iterable.hpp>
#include <boost/hana/detail/foldable_from_iterable.hpp>
#include <boost/hana/detail/integer_sequence.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    struct _Range;

    namespace operators {
        //! @ingroup datatypes
        template <typename T, T ...v>
        struct Range {
            using hana_datatype = _Range;
        };
    }
    using operators::Range;

    constexpr struct _range {
        template <typename T, T from, typename U, U to>
        constexpr auto operator()(Integral<T, from>, Integral<U, to>) const {
            return typename detail::make_integer_sequence<T, to - from>::
                   template slide_by<from, Range>{};
        }
    } range{};

    template <>
    struct Iterable<_Range> : defaults<Iterable> {
        template <typename T, T v, T ...vs>
        static constexpr auto head_impl(Range<T, v, vs...>)
        { return integral<T, v>; }

        template <typename T, T v, T ...vs>
        static constexpr Range<T, vs...> tail_impl(Range<T, v, vs...>)
        { return {}; }

        template <typename T, T ...vs>
        static constexpr auto is_empty_impl(Range<T, vs...>)
        { return bool_<sizeof...(vs) == 0>; }
    };

    template <>
    struct Foldable<_Range> : detail::foldable_from_iterable {
        template <typename F, typename T, T ...vs>
        static constexpr auto unpack_impl(F f, Range<T, vs...>)
        { return f(Integral<T, vs>{}...); }
    };

    template <>
    struct Comparable<_Range, _Range> : defaults<Comparable> {
        template <typename T, T ...ts, typename U, U ...us>
        static constexpr Bool<false>
        equal_helper(Range<T, ts...>, Range<U, us...>, Bool<false>)
        { return {}; }

        template <typename X, X ...xs, typename Y, Y ...ys>
        static constexpr auto
        equal_helper(Range<X, xs...>, Range<Y, ys...>, Bool<true>)
        { return and_(bool_<xs == ys>...); }

        template <typename T, T ...ts, typename U, U ...us>
        static constexpr auto equal_impl(Range<T, ts...> a, Range<U, us...> b)
        { return equal_helper(a, b, bool_<sizeof...(ts) == sizeof...(us)>); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RANGE_HPP
