/*!
@file
Forward declares `boost::hana::Range`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_RANGE_HPP
#define BOOST_HANA_FWD_RANGE_HPP

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/fwd/comparable.hpp>
#include <boost/hana/fwd/constant.hpp>
#include <boost/hana/fwd/integral_constant.hpp>
#include <boost/hana/fwd/iterable.hpp>
#include <boost/hana/fwd/orderable.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Compile-time half-open interval of `Integral`s.
    //!
    //! A `Range` represents a half-open interval of the form `[from, to)`
    //! containing `Integral`s. The notation `[from, to)` represents the
    //! values starting at `from` (inclusively) up to but excluding `from`.
    //! In other words, it is a bit like the list `from, from+1, ..., to-1`.
    //!
    //! In particular, note that the bounds of the range can be any
    //! `Integral`s (negative numbers are allowed) and the range does
    //! not have to start at zero. The only requirement is that `from <= to`.
    //!
    //! Also note that because `Range`s do not specify much about their actual
    //! representation, some interesting optimizations can be applied to
    //! improve their compile-time performance over other similar utilities
    //! like `std::integer_sequence`.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Comparable` (operators provided)\n
    //! Two ranges are equal if and only if they are both empty or they both
    //! span the same interval.
    //! @snippet example/range.cpp comparable
    //!
    //! 2. `Foldable`\n
    //! Folding a `Range` is equivalent to folding the list of the `Integral`
    //! values in the interval it spans.
    //! @snippet example/range.cpp foldable
    //!
    //! 3. `Iterable` (operators provided)\n
    //! Iterating over a `Range` is equivalent to iterating over a list of
    //! the values it spans. In other words, iterating over the range
    //! `[from, to)` is equivalent to iterating over a list containing
    //! `from, from+1, from+2, ..., to-1`.
    //! @snippet example/range.cpp iterable
    struct Range {
#ifndef BOOST_HANA_DOXYGEN_INVOKED
        struct hana {
            struct enabled_operators
                : Comparable
                , Iterable
            { };
        };
#endif
    };

    //! Creates a `Range` representing the half-open interval of
    //! `Integral`s `[from, to)`.
    //! @relates Range
    //!
    //! `from` and `to` must be `Integral`s such that `from <= to`.
    //! Otherwise, a compilation error is triggered.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/range.cpp range
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto range = [](auto from, auto to) {
        return unspecified-type;
    };
#else
    template <typename From, typename To>
    struct _range
        : operators::enable_adl
        , operators::Iterable_ops<_range<From, To>>
    {
        constexpr _range(From f, To t) : from(f), to(t) {
            auto valid_range = less_equal(from, to);
            static_assert(value(valid_range),
            "invalid usage of boost::hana::range(from, to) with from > to");
        }
        From from;
        To to;
        struct hana { using datatype = Range; };
    };

    constexpr detail::create<_range> range{};
#endif

    //! Shorthand to create a `Range` of `Constant`s.
    //! @relates Range
    //!
    //! This shorthand is provided for convenience only and it is equivalent
    //! to `range`. Specifically, `range_c<T, from, to>` is such that
    //! @code
    //!     range_c<T, from, to> == range(integral_constant<T, from>, integral_constant<T, to>)
    //! @endcode
    //!
    //!
    //! @tparam T
    //! The underlying integral type of the `IntegralConstant`s in the created
    //! range.
    //!
    //! @tparam from
    //! The inclusive lower bound of the created range.
    //!
    //! @tparam to
    //! The exclusive upper bound of the created range.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/range.cpp range_c
    template <typename T, T from, T to>
    constexpr auto range_c = range(integral_constant<T, from>,
                                   integral_constant<T, to>);
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_RANGE_HPP
