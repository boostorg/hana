/*!
@file
Forward declares `boost::hana::take` and `boost::hana::take_c`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_TAKE_HPP
#define BOOST_HANA_FWD_TAKE_HPP

#include <boost/hana/core/when.hpp>

#include <cstddef>


namespace boost { namespace hana {
    //! Returns the first `n` elements of a sequence, or the whole sequence
    //! if the sequence has less than `n` elements.
    //! @relates Sequence
    //!
    //! Given a `Sequence` `xs` and an `IntegralConstant` `n`, `take(xs, n)`
    //! is a new sequence containing the first `n` elements of `xs`, in the
    //! same order. If `length(xs) <= n`, the whole sequence is returned and
    //! no error is triggered.
    //!
    //!
    //! @param xs
    //! The sequence to take the elements from.
    //!
    //! @param n
    //! A non-negative `IntegralConstant` representing the number of elements
    //! to keep in the resulting sequence.
    //!
    //!
    //! Example
    //! -------
    //! @include example/take.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto take = [](auto&& xs, auto const& n) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct take_impl : take_impl<S, when<true>> { };

    struct take_t {
        template <typename Xs, typename N>
        constexpr auto operator()(Xs&& xs, N const& n) const;
    };

    constexpr take_t take{};
#endif

    //! Equivalent to `take`; provided for convenience.
    //! @relates Sequence
    //!
    //!
    //! Example
    //! -------
    //! @include example/take_c.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto take_c = [](auto&& xs) {
        return hana::take(forwarded(xs), hana::size_c<n>);
    };
#else
    template <std::size_t n>
    struct take_c_t;

    template <std::size_t n>
    constexpr take_c_t<n> take_c{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TAKE_HPP
