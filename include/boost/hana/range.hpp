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
    /*!
    @ingroup datatypes
    Compile-time half-open interval of `Integral`s.

    --------------------------------------------------------------------------

    ## Instance of

    ### Iterable
    Let `r` be a `Range` containing the `Integral`s in the half-open interval
    `[from, to)`. The head of `r` is an `Integral` with value `from`, its tail
    is the range representing the `[from + 1, to)` interval and `r` is empty
    if and only if `from == to`.
    @snippet example/range/iterable.cpp main

    ### Foldable
    Generic instance for `Iterable`s.

    ### Comparable
    Generic instance for `Iterable`s.
     */
    struct Range { };

    namespace operators {
        template <typename From, typename To>
        struct _range {
            From from;
            To to;
            using hana_datatype = Range;
        };
    }

    //! Creates a `Range` containing the `Integral`s in `[from, to)`.
    //! @relates Range
    //! @hideinitializer
    //!
    //! `from` and `to` must be `Integral`s such that `from <= to`. Otherwise,
    //! a compilation error is triggered.
    //!
    //! ### Example
    //! @snippet example/range/range.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto range = [](auto from, auto to) {
        // For some reason, Clang 3.5 requires that we create an intermediate
        // variable whose type is dependent so we can use `valid_range` as a
        // constant expression below.
        auto valid_range = from <= to;
        static_assert(valid_range,
        "invalid usage of boost::hana::range(from, to) with from > to");
        return operators::_range<decltype(from), decltype(to)>{from, to};
    };

    template <>
    struct Iterable<Range> : defaults<Iterable>::with<Range> {
        template <typename R>
        static constexpr auto head_impl(R r)
        { return r.from; }

        template <typename R>
        static constexpr auto tail_impl(R r)
        { return range(r.from + int_<1>, r.to); }

        template <typename R>
        static constexpr auto is_empty_impl(R r)
        { return r.from == r.to; }

        template <typename N, typename R>
        static constexpr auto at_impl(N n, R r)
        { return r.from + n; }

        template <typename R>
        static constexpr auto last_impl(R r)
        { return r.to - int_<1>; }

        template <typename N, typename R>
        static constexpr auto drop_impl(N n, R r) {
            auto size = r.to - r.from;
            return range(if_(n > size, r.to, r.from + n), r.to);
        }
    };

    template <>
    constexpr bool foldable_from_iterable<Range> = true;

    template <>
    struct Foldable<Range> : instance<Foldable>::with<Range> {
        template <typename F, typename From, typename T, T ...vs>
        static constexpr auto unpack_helper(F f, From from, detail::integer_sequence<T, vs...>)
        { return f(integral<T, from + vs>...); }

        template <typename F, typename R>
        static constexpr auto unpack_impl(F f, R r) {
            // For some reason, Clang 3.5 requires that we create an intermediate
            // variable whose type is dependent so we can use `size` as a
            // constant expression below.
            auto size = r.to - r.from;
            return unpack_helper(f, r.from, detail::make_integer_sequence<decltype(value(r.from)), size>{});
        }

        template <typename R>
        static constexpr auto length_impl(R r)
        { return r.to - r.from; }
    };

    template <>
    constexpr bool comparable_from_iterable<Range> = true;

    template <>
    struct Comparable<Range, Range>
        : defaults<Comparable>::template with<Range, Range>
    {
        template <typename R1, typename R2>
        static constexpr auto equal_impl(R1 r1, R2 r2) {
            return (is_empty(r1) && is_empty(r2)) ||
                   (r1.from == r2.from && r1.to == r2.to);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RANGE_HPP
