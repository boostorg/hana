/*!
@file
Forward declares `boost::hana::drop_front_exactly`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_DROP_FRONT_EXACTLY_HPP
#define BOOST_HANA_FWD_DROP_FRONT_EXACTLY_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Drop the first `n` elements of an iterable, and return the rest.
    //! @relates Iterable
    //!
    //! Given an `Iterable` `xs` with a linearization of `[x1, x2, ...]` and
    //! a (non-negative) `Constant` `n` holding an unsigned integral value,
    //! `drop_front_exactly(xs, n)` is an iterable of the same data type whose
    //! linearization is `[xn+1, xn+2, ...]`. In particular, note that this
    //! function does not mutate the original iterable in any way. If `n` is
    //! not given, it defaults to a `Constant` with an unsigned integral value
    //! equal to `1`.
    //!
    //! It is an error to use `drop_front_exactly` with `n > length(xs)`. This
    //! additional guarantee allows `drop_front_exactly` to be better optimized
    //! than the `drop_front` function, which allows `n > length(xs)`.
    //!
    //!
    //! @param xs
    //! The iterable from which elements are dropped.
    //!
    //! @param n
    //! A non-negative `Constant` holding an unsigned integral value
    //! representing the number of elements to be dropped from the iterable.
    //! `n` must be less than or equal to the number of elements in `xs`.
    //! If `n` is not given, it defaults to a `Constant` with an unsigned
    //! integral value equal to `1`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp drop_front_exactly
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto drop_front_exactly = [](auto&& xs[, auto&& n]) {
        return tag-dispatched;
    };
#else
    template <typename It, typename = void>
    struct drop_front_exactly_impl : drop_front_exactly_impl<It, when<true>> { };

    struct drop_front_exactly_t {
        template <typename Xs, typename N>
        constexpr auto operator()(Xs&& xs, N&& n) const;

        template <typename Xs>
        constexpr auto operator()(Xs&& xs) const;
    };

    constexpr drop_front_exactly_t drop_front_exactly{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_DROP_FRONT_EXACTLY_HPP
