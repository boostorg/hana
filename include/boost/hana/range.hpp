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

    BOOST_HANA_CONSTEXPR_LAMBDA auto range = [](auto from, auto to) {
        // For some reason, Clang 3.5 requires that we create an intermediate
        // variable whose type is dependent so we can use `size` as a template
        // parameter below.
        auto size = to - from;
        return typename detail::make_integer_sequence<
            decltype(value(from)), size
        >::template slide_by<from, Range>{};
    };

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
        { return f(integral<T, vs>...); }
    };

    template <>
    struct Comparable<_Range, _Range> : defaults<Comparable> {
        // SFINAE handles the case where sizeof...(ts) != sizeof...(us).
        template <typename T, T ...ts, typename U, U ...us>
        static constexpr auto equal_impl(Range<T, ts...>, Range<U, us...>)
            -> decltype(and_(bool_<(ts == us)>...))
        { return {}; }

        static constexpr auto equal_impl(...)
        { return false_; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RANGE_HPP
