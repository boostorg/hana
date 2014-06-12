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
#include <boost/hana/detail/integer_sequence.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    //! @datatype{Range}
    //! Compile-time half-open interval of `Integral`s.
    //!
    //! @instantiates{Iterable, Foldable, Comparable}
    //!
    //! @todo Document instances.
    //! @todo The implementation could be more clever: only unpack when we
    //! need to, but don't encode it in the type of the range.
    struct Range { };

    namespace operators {
        template <typename T, T ...v>
        struct _range { using hana_datatype = Range; };
    }

    //! Creates a `Range` containing the integers in `[from, to)`.
    //! @relates Range
    BOOST_HANA_CONSTEXPR_LAMBDA auto range = [](auto from, auto to) {
        // For some reason, Clang 3.5 requires that we create an intermediate
        // variable whose type is dependent so we can use `size` as a template
        // parameter below.
        auto size = to - from;
        return typename detail::make_integer_sequence<
            decltype(value(from)), size
        >::template slide_by<from, operators::_range>{};
    };

    template <>
    struct Iterable<Range> : defaults<Iterable>::with<Range> {
        template <typename T, T v, T ...vs>
        static constexpr auto head_impl(operators::_range<T, v, vs...>)
        { return integral<T, v>; }

        template <typename T, T v, T ...vs>
        static constexpr operators::_range<T, vs...>
        tail_impl(operators::_range<T, v, vs...>)
        { return {}; }

        template <typename T, T ...vs>
        static constexpr auto is_empty_impl(operators::_range<T, vs...>)
        { return bool_<sizeof...(vs) == 0>; }
    };

    template <>
    constexpr bool foldable_from_iterable<Range> = true;

    template <>
    struct Foldable<Range> : instance<Foldable>::with<Range> {
        template <typename F, typename T, T ...vs>
        static constexpr auto unpack_impl(F f, operators::_range<T, vs...>)
        { return f(integral<T, vs>...); }
    };

    template <>
    constexpr bool comparable_from_iterable<Range> = true;

    template <>
    struct Comparable<Range, Range>
        : defaults<Comparable>::template with<Range, Range>
    {
        // SFINAE handles the case where sizeof...(ts) != sizeof...(us).
        template <typename T, T ...ts, typename U, U ...us>
        static constexpr auto
        equal_impl(operators::_range<T, ts...>, operators::_range<U, us...>)
            -> decltype(and_(bool_<(ts == us)>...))
        { return {}; }

        static constexpr auto equal_impl(...)
        { return false_; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RANGE_HPP
