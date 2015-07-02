/*!
@file
Forward declares `boost::hana::drop_back_exactly`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_DROP_BACK_EXACTlY_HPP
#define BOOST_HANA_FWD_DROP_BACK_EXACTlY_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Drop the last `n` elements of a finite sequence, and return the rest.
    //! @relates Sequence
    //!
    //! Given a finite `Sequence` `xs` with a linearization of `[x1, ..., xm]`
    //! and a non-negative `Constant` `n` holding an unsigned integral value,
    //! `drop_back_exactly(xs, n)` is a sequence with the same tag as `xs`
    //! whose linearization is `[x1, ..., xm-n]`. If `n` is not given, it
    //! defaults to a `Constant` with an unsigned integral value equal to `1`.
    //!
    //! It is an error to use `drop_back_exactly` with `n > length(xs)`. This
    //! additional guarantee allows `drop_back_exactly` to be better optimized
    //! than the `drop_back` function, which allows `n > length(xs)`.
    //!
    //!
    //! @param xs
    //! The sequence from which elements are dropped.
    //!
    //! @param n
    //! A non-negative `Constant` holding an unsigned integral value
    //! representing the number of elements to be dropped from the end
    //! of the sequence. `n` must be less than or equal to the number of
    //! elements in `xs`. If `n` is not given, it defaults to a `Constant`
    //! with an unsigned integral value equal to `1`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp drop_back_exactly
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto drop_back_exactly = [](auto&& xs[, auto&& n]) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct drop_back_exactly_impl : drop_back_exactly_impl<S, when<true>> { };

    struct drop_back_exactly_t {
        template <typename Xs, typename N>
        constexpr auto operator()(Xs&& xs, N&& n) const;

        template <typename Xs>
        constexpr auto operator()(Xs&& xs) const;
    };

    constexpr drop_back_exactly_t drop_back_exactly{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_DROP_BACK_EXACTlY_HPP
