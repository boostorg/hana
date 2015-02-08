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
#include <boost/hana/fwd/integral.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Compile-time half-open interval of `Constant`s.
    //!
    //! A `Range` represents a half-open interval of the form `[from, to)`
    //! containing `Constant`s of an integral type. The notation `[from, to)`
    //! represents the values starting at `from` (inclusively) up to but
    //! excluding `from`. In other words, it is a bit like the list
    //! `from, from+1, ..., to-1`.
    //!
    //! In particular, note that the bounds of the range can be any
    //! `Constant`s (negative numbers are allowed) and the range does
    //! not have to start at zero. The only requirement is that `from <= to`,
    //! and that the `Constant`s are holding an integral type.
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
    //! Folding a `Range` is equivalent to folding a list of the `Constant`s
    //! in the interval it spans.
    //! @snippet example/range.cpp foldable
    //!
    //! 3. `Iterable` (operators provided)\n
    //! Iterating over a `Range` is equivalent to iterating over a list of
    //! the values it spans. In other words, iterating over the range
    //! `[from, to)` is equivalent to iterating over a list containing
    //! `from, from+1, from+2, ..., to-1`.
    //! @snippet example/range.cpp iterable
    struct Range { };

    //! Creates a `Range` representing the half-open interval of
    //! `Constant`s `[from, to)`.
    //! @relates Range
    //!
    //! `from` and `to` must be `Constant`s of an integral type and such that 
    //! `from <= to`. Otherwise, a compilation error is triggered. Also note
    //! that if `from` and `to` are `Constant`s with different underlying
    //! integral types, the created range contains `Constant`s whose
    //! underlying type is the common type of the two underlying types.
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
    struct _range;

    struct _make_range {
        template <typename From, typename To>
        constexpr auto operator()(From from, To to) const;
    };

    constexpr _make_range range{};
#endif

    //! Shorthand to create a `Range` of `Constant`s.
    //! @relates Range
    //!
    //! This shorthand is provided for convenience only and it is equivalent
    //! to `range`. Specifically, `range_c<T, from, to>` is such that
    //! @code
    //!     range_c<T, from, to> == range(integral<T, from>, integral<T, to>)
    //! @endcode
    //!
    //!
    //! @tparam T
    //! The underlying integral type of the `Constant`s in the created range.
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
    constexpr auto range_c = range(integral<T, from>, integral<T, to>);
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_RANGE_HPP
