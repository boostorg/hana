/*!
@file
Forward declares `boost::hana::slice` and `boost::hana::slice_c`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_SLICE_HPP
#define BOOST_HANA_FWD_SLICE_HPP

#include <boost/hana/core/when.hpp>

#include <cstddef>


namespace boost { namespace hana {
    //! Extract a subsequence delimited by the given indices.
    //! @relates Sequence
    //!
    //! Specifically, `slice(xs, from, to)` is a sequence containing all the
    //! elements of `xs` at indices in the half-open interval delimited by
    //! [`from`, `to`). Note that the indices are 0-based. For this operation
    //! to be valid, `xs` must contain at least `to + 1` elements, and it must
    //! be true that `from <= to`.
    //!
    //!
    //! @param xs
    //! The sequence to slice.
    //!
    //! @param from
    //! The index of the first element in the slice. `from` must be a
    //! non-negative `Constant` of an unsigned integral type.
    //!
    //! @param to
    //! One-past the index of the last element in the slice. `to` must be
    //! a non-negative `Constant` of an unsigned integral type such that
    //! `from <= to`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp slice
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto slice = [](auto&& xs, auto&& from, auto&& to) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct slice_impl : slice_impl<S, when<true>> { };

    struct slice_t {
        template <typename Xs, typename From, typename To>
        constexpr auto operator()(Xs&& xs, From&& from, To&& to) const;
    };

    constexpr slice_t slice{};
#endif

    //! Equivalent to `slice`; provided for convenience.
    //! @relates Sequence
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp slice_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t from, std::size_t to>
    constexpr auto slice_c = [](auto&& xs) {
        return hana::slice(forwarded(xs), hana::size_t<from>, hana::size_t<to>);
    };
#else
    template <std::size_t from, std::size_t to>
    struct slice_c_t;

    template <std::size_t from, std::size_t to>
    constexpr slice_c_t<from, to> slice_c{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_SLICE_HPP
