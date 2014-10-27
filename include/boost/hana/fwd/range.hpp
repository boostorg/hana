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
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/fwd/comparable.hpp>
#include <boost/hana/fwd/iterable.hpp>
#include <boost/hana/fwd/orderable.hpp>
#include <boost/hana/integral.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Compile-time half-open interval of `Integral`s.
    //!
    //! ## Instance of
    //! `Iterable`, `Foldable`, `Comparable`
    //!
    //! @todo
    //! Remove the requirement that the range contains `Integral`s, and allow
    //! any `Orderable` and `Comparable` `Ring`, or something similar.
    struct Range {
        struct hana_enabled_operators
            : Comparable, Iterable
        { };
    };

    namespace range_detail {
        template <typename From, typename To, typename = operators::enable_adl>
        struct range : operators::Iterable_ops<range<From, To>> {
            constexpr range(From f, To t) : from(f), to(t) { }
            From from;
            To to;
            struct hana { using datatype = Range; };
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
    //! @snippet example/range.cpp range
    BOOST_HANA_CONSTEXPR_LAMBDA auto range = [](auto from, auto to) {
        // For some reason, Clang 3.5 requires that we create an intermediate
        // variable whose type is dependent so we can use `valid_range` as a
        // constant expression below.
        auto valid_range = less_equal(from, to);
        static_assert(valid_range(),
        "invalid usage of boost::hana::range(from, to) with from > to");
        return range_detail::range<decltype(from), decltype(to)>{from, to};
    };

    //! Shorthand to create a `Range` of `Integral`s.
    //! @relates Range
    //!
    //! This shorthand is provided for convenience only and it is equivalent
    //! to using `range`. Specifically, `range_c<T, from, to>` is such that
    //! @code
    //!     range_c<T, from, to> == range(integral<T, from>, integral<T, to>)
    //! @endcode
    //!
    //!
    //! @tparam T
    //! The underlying integral type of the `Integral`s in the created range.
    //!
    //! @tparam from
    //! The inclusive lower bound of the created range.
    //!
    //! @tparam to
    //! The exclusive upper bound of the created range.
    //!
    //!
    //! ### Example
    //! @snippet example/range.cpp range_c
    template <typename T, T from, T to>
    BOOST_HANA_CONSTEXPR_LAMBDA auto range_c = range(integral<T, from>, integral<T, to>);
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_RANGE_HPP
